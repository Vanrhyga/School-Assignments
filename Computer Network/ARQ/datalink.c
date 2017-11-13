#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"


#define DATA_TIMER  4000						//��ʱ��
#define MAX_SEQ		31							//����֡�����ţ�����ʼ��
#define NR_BUFS		((MAX_SEQ + 1) / 2)			//���ڼ���������С
#define ACK_TIMER	2500						//���� ACK ��ʱ��


typedef struct FRAME {							//֡�ṹ
	unsigned char kind;							//֡����
	unsigned char ack;							//���� ACK ���
	unsigned char seq;							//֡���
	unsigned char data[PKT_LEN];				//��������ݰ�����
	unsigned int padding;						//����ֶΣ�CRC У�飩
}FRAME;


int no_nak = 1;									//�Ƿ��ѷ��͹� NAK
static int phl_ready = 0;						//������Ƿ����


static int between(unsigned char, unsigned char, unsigned char);										//�ж��Ƿ��ڴ�����
static void put_frame(unsigned char *, int);
static void send_data_frame(unsigned char, unsigned char, unsigned char, unsigned char[][PKT_LEN]);


int main(int argc,char **argv) {
	unsigned char out_buf[NR_BUFS][PKT_LEN];				//���ͷ�������
	unsigned char in_buf[NR_BUFS][PKT_LEN];					//���շ�������
	static unsigned char frame_nr = 0, nbuffered = 0;
	static unsigned char frame_expected = 0, ack_expected = 0, next_frame_to_send = 0, too_far = NR_BUFS;
	int arg, i, len = 0,event;
	int arrived[NR_BUFS];									//��־λ����
	FRAME f;
	for (i = 0; i < NR_BUFS; i++)							//��ʼ����־λ����
		arrived[i] = 0;

	protocol_init(argc, argv);								//��ʼ��Э�����л���
	lprintf("Designed by XXX, build: "__DATE__"  "__TIME__"\n");

	disable_network_layer();

	while (1) {
		event = wait_for_event(&arg);							//���̵ȴ���ֱ���¼�����

		switch (event){
		case NETWORK_LAYER_READY:								//������д����͵ķ���
			get_packet(&out_buf[next_frame_to_send%NR_BUFS]);
			nbuffered++;										//�ѷ��͵�δ�յ� ACK ��֡��������
			send_data_frame(FRAME_DATA, next_frame_to_send, frame_expected, out_buf);
			next_frame_to_send = (next_frame_to_send + 1) % (MAX_SEQ + 1);
			break;

		case PHYSICAL_LAYER_READY:								//��������
			phl_ready = 1;
			break;

		case FRAME_RECEIVED:													//������յ�֡
			len = recv_frame((unsigned char *)&f, sizeof f);
			if (len < 6 || crc32((unsigned char *)&f, len) != 0) {				//֡����
				dbg_event("**** Receiver Error, Bad CRC Checksum\n");
				if (no_nak)														//��δ���� NAK
					send_data_frame(FRAME_NAK, 0, frame_expected, out_buf);
				break;
			}
			if (f.kind == FRAME_DATA) {											//���յ�֡Ϊ����֡
				dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
				if ((f.seq != frame_expected) && no_nak)
					send_data_frame(FRAME_NAK, 0, frame_expected, out_buf);
				else
					start_ack_timer(ACK_TIMER);
				if (between(frame_expected, f.seq, too_far) && (arrived[f.seq%NR_BUFS] == 0)) {
					arrived[f.seq%NR_BUFS] = 1;
					memcpy(in_buf[f.seq%NR_BUFS], f.data, PKT_LEN);
					while (arrived[frame_expected%NR_BUFS]) {					//�������˳�򴫵�����
						put_packet(&in_buf[frame_expected%NR_BUFS], len - 7);
						no_nak = 1;
						arrived[frame_expected%NR_BUFS] = 0;
						frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);
						too_far = (too_far + 1) % (MAX_SEQ + 1);
						start_ack_timer(ACK_TIMER);
					}
				}
			}
			if (f.kind == FRAME_NAK&&between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send)) {		//���յ� NAK
				dbg_frame("recv nak\n");
				send_data_frame(FRAME_DATA, (f.ack + 1) % (MAX_SEQ + 1), frame_expected, out_buf);					//�ش�
			}
			if (f.kind == FRAME_ACK)																				//���յ� ACK
				dbg_event("recv ack %d\n", f.ack);
			while (between(ack_expected, f.ack, next_frame_to_send)) {
				nbuffered--;
				stop_timer(ack_expected%NR_BUFS);
				ack_expected = (ack_expected + 1) % (MAX_SEQ + 1);
			}
			break;

		case DATA_TIMEOUT:																							//��ʱ����ʱ
			dbg_event("---- DATA %d timeout\n", arg);
			if (!between(ack_expected, arg, next_frame_to_send))
				arg += NR_BUFS;
			send_data_frame(FRAME_DATA, arg, frame_expected, out_buf);												//�ش�
			break;

		case ACK_TIMEOUT:
			send_data_frame(FRAME_ACK, 0, frame_expected, out_buf);													//���� ACK ��ʱ����ʱ
		}

		if (nbuffered < NR_BUFS&&phl_ready)
			enable_network_layer();
		else
			disable_network_layer();
	}
}


static int between(unsigned char a, unsigned char b, unsigned char c) {
	return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}


static void put_frame(unsigned char *frame, int len) {
	*(unsigned int *)(frame + len) = crc32(frame, len);			//���У���ֶ�
	send_frame(frame, len + 4);
	phl_ready = 0;
}


static void send_data_frame(unsigned char fk, unsigned char frame_nr, unsigned char frame_expected, unsigned char buffer[][PKT_LEN]) {
	FRAME s;

	s.kind = fk;
	if (fk == FRAME_DATA)					//��������֡Ϊ����֡
		memcpy(s.data, buffer[frame_nr%NR_BUFS], PKT_LEN);
	s.seq = frame_nr;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
	if (fk == FRAME_NAK)					//��������֡Ϊ NAK
		no_nak = 0;

	if (fk == FRAME_DATA)
		dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);

	put_frame((unsigned char *)&s, 3 + PKT_LEN);
	if (fk == FRAME_DATA)					//������ʱ��
		start_timer(frame_nr%NR_BUFS, DATA_TIMER);
	stop_ack_timer();
}
