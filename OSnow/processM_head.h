#pragma once
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<ctime>
#include<sstream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
using namespace std;
/* ע�����ӽ��̼���ɾ��;
�������н���Ϊ����������Ԫ��
*/


/*����*/
#define MAX_RESOURCE_AMOUNT 3
#define MAX_SIZE			12
#define MAX_RUNTIME			27


/*ö��*/
enum processState {								//����״̬
	ready,running,blocked
};
enum listType {									//��������
	readyL,blockedL
};
enum processType {								//��������
	user,forSystem
};
enum processOperate {							//���̲���
	request,release,timeout,dispatch
};


/*�ṹ*/
typedef struct processControlBlock {			//���̿��ƿ�
	string PID;									//���̱�ʶ
	string name;								//��������
	string parentPID = "";						//�����̱�ʶ
	int runtime = rand() % MAX_RUNTIME + 1;
	processState state = ready;					//����״̬
	processType type;							//��������
	listType list = readyL;						//���ڶ���
	map<string, int> resources;					//��ռ����Դ
	map<string, processControlBlock> childProcess;						//�ӽ���
	processControlBlock();
	processControlBlock(string PID, string name, processType type);
	void createChildP(string PID, string name, processType type);		//�����ӽ���
	void increaseResource(string RID, int amount);						//����������Դ
	int countResource(string RID);				//ͳ����Դ
	void releaseAllResource(string *s);
}PCB;
struct processInfo {							//������Ϣ
	string PID;									//���̱�ʶ
	int reqAmount;								//������Դ��
};
struct resource{								//��Դ
	string RID;									//��Դ��ʶ
	int amount;									//��Դ����
	int freeAmount;								//������Դ��
	vector<processInfo> waitingL;				//�ȴ�����
	resource(string RID, int amount, int freeAmount);
	int request(string PID, int amount);		//������Դ
	string release(int amount, string PID);		//�ͷ���Դ
};


/*ȫ�ֱ���*/
extern list<string> readyList[2];				//��������
extern list<string> blockedList[2];				//��������
extern map<string, PCB> process;
extern map<string, resource> allResource;
extern int counter;
extern int timeSlot;
extern bool isRunning;
extern ofstream ofp;


/*���ܺ���*/
void initList();								//��ʼ������
PCB& getProcess(string PID);
resource& getResource(string RID);
void insertProcess(string PID, PCB p);
void outProcess(string PID);
void destroyProcess(string PID);				//ɾ������
void killProcess(string PID);
void insertRL(string PID, processType type);	//�����������
void outRL(processType type);					//�Ƴ���������
void insertBL(string PID, processType type);	//������������
void outBL(string PID, processType type);		//�Ƴ���������
void contextSwitch(processType type);			//�������л�
void intoRunning(processType type);				//�л�����״̬
void outOfRunning(string PID, processType type, processState state);	//��������״̬
string getRunningProcess();						//��ȡ��ǰִ�н���
void dispatcher();								//����
void RR();
void pri();
void annotation();
void recordTime();


/*���ߺ���*/
string toString(int i);
DWORD WINAPI TIMER_SEC(LPVOID lpparentet);
DWORD WINAPI TIMER_NINE_SEC(LPVOID lpparentet);
void Screen();


/*Ƥһ��*/
void painting();