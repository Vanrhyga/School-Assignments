#include"mmu.h"

ofstream printlog;		//��־
size_t   memNum = MSIZE;//����ܴ�С
time_t   now;           //ʱ��
char*    dt;			//���ʱ���ַ�

memList block_first;		//ͷ���
memList block_last;			//β���

extern int allocMode;

Status Initblock()			
{
	block_first = new memNode;
	block_last = new memNode;
	block_first->prior = NULL;
	block_first->next = block_last;
	block_first->data.state = "3";
	block_first->data.size = 0;
	block_last->prior = block_first;
	block_last->next = NULL;
	block_last->data.address = 0;
	block_last->data.size = MSIZE;
	block_last->data.ID = "0";
	block_last->data.state = Free;
	return OK;
}

Status alloc(string ID,size_t request)
{
	size_t paddr;		//���صĵ�ַ
	assert(ID != "0");
	assert(request % PGSIZE == 0);
	switch (allocMode)
	{
	case 1:
		paddr = First_fit(ID, request);
		break;
	case 2:
		paddr = Best_fit(ID, request);
		break;
	case 3:
		paddr = Worst_fit(ID, request);
		break;
	default:
		break;
	}
	now = time(0);
	dt = ctime(&now);
	printlog.open("pm_log.txt", ios::app);
	if (paddr >= 0) {
		memNum -= request;//���ڴ����
		printlog << dt << "Succeed: Assigned successfully!      StartAddr��" << paddr << "      EndAddr��" << paddr + request << "      Memory��" << memNum << endl;
		printlog.close();
		return paddr;
	}
	else {
		printlog << dt << "Error: Insufficient Memory, Assigned failed!      " << "      Memory��" << memNum << endl;
		printlog.close();
		return ERROR;
	}
}

Status First_fit(string ID, size_t request)
{
	memNode *p = block_first->next;
	memList block = new memNode;
	memset(block, 0, sizeof(memNode));
	block->data.ID = ID;
	block->data.size = request;
	block->data.state = Busy;
	while (p)
	{
		if (p->data.state == Free && p->data.size >= request)
		{
			if ((p->data.size - request) > 1)
			{
				block->data.address = p->data.address;
				p->data.address = p->data.address + request;
				p->data.size = p->data.size - request;

				p->prior->next = block;
				block->next = p;
				block->prior = p->prior;
				p->prior = block;
				return block->data.address;//return paddr
			}
			else
			{
				p->data.ID = ID;
				p->data.state = Busy;
				delete block;
				return p->data.address;//return paddr
			}
		}
		p = p->next;
	}
	delete block;
	return ERROR;
}

Status Best_fit(string ID, size_t request)
{
	memList block = new memNode;
	memset(block, 0, sizeof(memNode));
	block->data.ID = ID;
	block->data.size = request;
	block->data.state = Busy;
	memNode *p = block_first->next;
	memNode *q = NULL; //��¼��Ѳ���λ��
	int i = 0;
	int num = 0;
	memNode *q1 = NULL;

	while (p)
	{
		if (p->data.state == Free && p->data.size >= request)
		{
			if (num == 0)
			{
				q = p;
				i = q->data.size - request;
			}
			else if (p->data.size - request < i)
			{
				q = p;
				i = q->data.size - request;
			}
			num++;
		}
		p = p->next;
	}
	//Ҫ���ҵ���Сʣ��ռ�ķ���������Ѳ���λ��
	if (q == NULL) return ERROR;//û���ҵ����п�
	else
	{
		//�ҵ������λ�ò�ʵ���ڴ����Ĵ���
		if ((q->data.size - request) > 1)
		{
			block->data.address = q->data.address;
			q->data.address = q->data.address + request;
			q->data.size = q->data.size - request;

			block->next = q;
			block->prior = q->prior;
			q->prior->next = block;
			q->prior = block;
			return block->data.address;
		}
		else
		{
			q->data.ID = ID;
			q->data.state = Busy;
			delete block;
			return q->data.address;
		}
	}
}

Status Worst_fit(string ID, size_t request) {
	memList block = new memNode;
	memset(block, 0, sizeof(memNode));
	block->data.ID = ID;
	block->data.size = request;
	block->data.state = Busy;
	memNode *p = block_first->next;
	memNode *q = NULL; //��¼��Ѳ���λ��
	int i = 0;
	int num = 0;
	memNode *q1 = NULL;

	while (p)
	{

		if (p->data.state == Free && p->data.size >= request)
		{
			if (num == 0)
			{
				q = p;
				i = q->data.size - request;
			}
			else if (p->data.size - request > i)
			{
				q = p;
				i = q->data.size - request;
			}
			num++;
		}
		p = p->next;
	}
	if (q == NULL) return ERROR;//û���ҵ����п�
	else
	{
		//�ҵ������λ�ò�ʵ���ڴ����Ĵ���
		if ((q->data.size - request) > 1)
		{
			block->data.address = q->data.address;
			q->data.address = q->data.address + request;
			q->data.size = q->data.size - request;

			block->next = q;
			block->prior = q->prior;
			q->prior->next = block;
			q->prior = block;
			return block->data.address;
		}
		else
		{
			q->data.ID = ID;
			q->data.state = Busy;
			delete block;
			return q->data.address;
		}
	}
}

Status free(string ID)
{
	bool result = false;
	memNode *p = block_first->next;
	while (p)
	{
		if (p->data.ID == ID)
		{
			result = true;
			p->data.state = Free;
			p->data.ID = Free;
			if (p == block_last) {
				if ((p->prior->data.state == Free) && (p->prior->data.address + p->prior->data.size == p->data.address))
				{//Ϊ���һ��
					p->prior->data.size += p->data.size;
					p->prior->next = NULL;
					free(p);
				}
				break;
			}
			//��������Ļ��յĴ��룬��Ҫ����Ҫ���յķ�����ǰ��Ŀ��п������������Ŀ��п�������������ǰ����п������ȡ�
			if ((p->next->next == NULL) && (p->next->data.state == Free) && (p->data.address + p->data.size == p->next->data.address))
			{//���һ������
				p->data.size += p->next->data.size;
				p->next = NULL;
				if ((p->prior->data.state == Free) && (p->prior->data.address + p->prior->data.size == p->data.address))
				{
					p->prior->data.size += p->data.size;
					p->prior->next = NULL;
					free(p);
				}
				break;
			}
			else if ((p->prior->data.state == Free) && (p->prior->data.address + p->prior->data.size == p->data.address))
			{//��ǰһ������

				if (p->next->data.state == Free && (p->data.address + p->data.size == p->next->data.address))
				{//���һ������
					p->data.size += p->next->data.size;
					p->next = p->next->next;
					p->next->prior = p;
				}
				p->prior->data.size += p->data.size;
				p->prior->next = p->next;
				p->next->prior = p->prior;
				free(p);
				break;
			}
			else if ((p->next->data.state == Free) && (p->data.address + p->data.size == p->next->data.address))
			{//ֻ���һ������

				p->data.size += p->next->data.size;
				p->next = p->next->next;
				p->next->prior = p;
				break;
			}
			break;
		}
		p = p->next;
	}
	now = time(0);
	dt = ctime(&now);
	printlog.open("pm_log.txt", ios::app);
	if (result)
	{
		memNum += p->data.size;
		//printlog << dt << "Succeed: Release Completed!      StartAddr��" << paddr << "      EndAddr��" << paddr + p->data.size << "      Memory��" << memNum << endl;
		printlog.close();
		return OK;
	}
	else {
		//printlog << dt << "Error: Release Failed!      Starting Address ��" << paddr << " is not found" << "      Memory��" << memNum << endl;
		printlog.close();
		return ERROR;
	}
}

char read_pm(size_t paddr)
{
	return 'a';
}

int write_pm(size_t paddr, char c)
{
	return 1;
}

void show()
{
	memNode *p = block_first->next;
	size_t used_mm = MSIZE - memNum;
	printf("------Physical Memory Layout %.2f%% Used-------\n",(used_mm*1.0/ MSIZE)*100);
	cout << "                                               " << endl;
	cout << "PID\tStartAddr\tEndAddr\t\tStatus" << endl;
	while (p)
	{
		if (p->data.ID == Free) cout << "Free\t";
		else if (p->data.ID == VM_USED) cout << "VM_USED\t";
		else cout << p->data.ID<<"\t";
		cout<< p->data.address<<"\t\t";
		cout<< p->data.size+ p->data.address << "\t\t";
		if (p->data.state == Free) cout << "Free" << endl;
		else cout << "Busy" << endl;
		p = p->next;
	}
}
