#include<iostream>
#include<fstream>
#include<ctime>
#include<assert.h>
#include"mmu.h"
#include"fs_head.h"
using namespace std;

ofstream printlog;		//日志
size_t   memNum = MSIZE;//物存总大小
time_t   now;           //时间
char*    dt;			//输出时间字符

memList block_first;		//头结点
memList block_last;			//尾结点

Status Initblock()				//初始化带头结点的内存空间链表
{
	block_first = (memList)malloc(sizeof(memNode));
	block_last = (memList)malloc(sizeof(memNode));
	block_first->prior = NULL;
	block_first->next = block_last;
	block_first->data.state = 3;
	block_first->data.size = 0;
	block_last->prior = block_first;
	block_last->next = NULL;
	block_last->data.address = 0;
	block_last->data.size = MSIZE;
	block_last->data.ID = 0;
	block_last->data.state = Free;
	return OK;
}

//------------------- 分 配 主 存 -----------------
//ID为-1时分配主存给VM，其他情况则分配给进程
Status alloc(int ID, size_t request)
{
	size_t paddr;		//返回的地址
	assert(ID == -1 || ID > 0);
	assert(request % PGSIZE == 0);
#ifdef FIRST_FIT
	paddr = First_fit(ID, request);
#elif BEST_FIT
	paddr = Best_fit(ID, request);
#elif WORST_FIT
	paddr = Worst_fit(ID, request);
#endif // FIRST_FIT
	now = time(0);
	dt = ctime(&now);
	printlog.open("log.txt", ios::app);
	if (paddr >= 0) {
		memNum -= request;//总内存减少
		//printlog << dt << "Succeed: Assigned successfully!      StartAddr：" << paddr << "      EndAddr：" << paddr + request << "      Memory：" << memNum << endl;
		printlog.close();
		return paddr;
	}
	else {
		//printlog << dt << "Error: Insufficient Memory, Assigned failed!      " << "      Memory：" << memNum << endl;
		printlog.close();
		return ERROR;
	}
}
//------------------ 首次适应算法 -----------------
Status First_fit(int ID, size_t request)//传入作业名及申请量 return :paddr
{
	memNode *p = block_first->next;
	memList block = (memList)malloc(sizeof(memNode));
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
				free(block);
				return p->data.address;//return paddr
			}
		}
		p = p->next;
	}
	free(block);
	return ERROR;
}
//--------------------最佳适应算法 ----------------
Status Best_fit(int ID, size_t request)
{
	memList block = (memList)malloc(sizeof(memNode));
	memset(block, 0, sizeof(memNode));
	block->data.ID = ID;
	block->data.size = request;
	block->data.state = Busy;
	memNode *p = block_first->next;
	memNode *q = NULL; //记录最佳插入位置
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
	//要查找到最小剩余空间的分区，即最佳插入位置
	if (q == NULL) return ERROR;//没有找到空闲块
	else
	{
		//找到了最佳位置并实现内存分配的代码
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
			free(block);
			return q->data.address;
		}
	}
}
//--------------------最差适应算法 ----------------
Status Worst_fit(int ID, size_t request) {
	//作业申请新空间且初始化的代码
	memList block = (memList)malloc(sizeof(memNode));
	memset(block, 0, sizeof(memNode));
	block->data.ID = ID;
	block->data.size = request;
	block->data.state = Busy;
	memNode *p = block_first->next;
	memNode *q = NULL; //记录最佳插入位置
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
	if (q == NULL) return ERROR;//没有找到空闲块
	else
	{
		//找到了最佳位置并实现内存分配的代码
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
			free(block);
			return q->data.address;
		}
	}
}

//--------------------主 存 回 收 -----------------
Status free(size_t paddr)
{
	bool result = false;
	memNode *p = block_first->next;
	while (p)
	{
		if (p->data.address == paddr)
		{
			result = true;
			p->data.state = Free;
			p->data.ID = Free;
			//cout << "内存块找到，准备回收！" << endl;
			if (p == block_last) {
				if ((p->prior->data.state == Free) && (p->prior->data.address + p->prior->data.size == p->data.address))
				{//为最后一块
					p->prior->data.size += p->data.size;
					p->prior->next = NULL;
					free(p);
				}
				//cout << "内存块为最后一块！" << endl;
				break;
			}
			//其他情况的回收的代码，主要包括要回收的分区与前面的空闲块相连或与后面的空闲块相连，或者与前后空闲块相连等。
			if ((p->next->next == NULL) && (p->next->data.state == Free) && (p->data.address + p->data.size == p->next->data.address))
			{//与后一块相连
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
			{//与前一块相连

				if (p->next->data.state == Free && (p->data.address + p->data.size == p->next->data.address))
				{//与后一块相连
					p->data.size += p->next->data.size;
					p->next = p->next->next;
					p->next->prior = p;
					//free(p->next);
				}
				p->prior->data.size += p->data.size;
				p->prior->next = p->next;
				p->next->prior = p->prior;
				free(p);
				break;
			}
			else if ((p->next->data.state == Free) && (p->data.address + p->data.size == p->next->data.address))
			{//只与后一块相连

				p->data.size += p->next->data.size;
				p->next = p->next->next;
				p->next->prior = p;
				//free(p->next);
				break;
			}
			break;
		}
		p = p->next;
	}
	now = time(0);
	dt = ctime(&now);
	printlog.open("log.txt", ios::app);
	if (result)
	{
		memNum += p->data.size;
		//printlog << dt << "Succeed: Release Completed!      StartAddr：" << paddr << "      EndAddr：" << paddr + p->data.size << "      Memory：" << memNum << endl;
		printlog.close();
		return OK;
	}
	else {
		//printlog << dt << "Error: Release Failed!      Starting Address ：" << paddr << " is not found" << "      Memory：" << memNum << endl;
		printlog.close();
		return ERROR;
	}
}

//--------------------读 物  存-------------------
char read_pm(size_t paddr)
{
	return 'a';
}
//--------------------写 物 存--------------------
int write_pm(size_t paddr, char c)
{
	return 1;
}

//---------------  显示主存分配情况 ----------------
void show()
{
	memNode *p = block_first->next;
	cout << "PID\tStartAddr\tEndAddr\t\tStatus" << endl;
	while (p)
	{
		if (p->data.ID == Free) cout << "Free\t";
		else if (p->data.ID == VM_USED) cout << "VM_USED\t";
		else cout << p->data.ID << "\t";
		cout << p->data.address << "\t\t";
		cout << p->data.size + p->data.address << "\t\t";
		if (p->data.state == Free) cout << "Free" << endl;
		else cout << "Busy" << endl;
		p = p->next;
	}
}

//------------------ 测 试 主  函  数--------------
//void main()
//{
//#ifdef FIRST_FIT
//	cout << "|-----------------FirstFit-----------------|" << endl;
//#elif BEST_FIT
//	cout << "|------------------BestFit-----------------|" << endl;
//#elif WORST_FIT
//	cout << "|------------------WorstFit----------------|" << endl;
//#endif // FIRST_FIT
//	Initblock(); //初始化空闲空间表
//	while (1)
//	{
//		int choice;
//		cout << "|------------------------------------------|\n";
//		cout << "|         1: alloc        2: free          |\n";
//		cout << "|         3: check        0: exit          |\n";
//		cout << "|------------------------------------------|\n";
//		cout << ">>";
//		cin >> choice;
//		if (choice == 1) {				// 分配内存
//			int ID;
//			size_t request = PGSIZE;
//			cout << "please enter the ID of job(-1 for VM others for pid)\n>>";
//			cin >> ID;
//			alloc(ID, request);
//		}
//		else if (choice == 2)			// 内存回收
//		{
//			size_t paddr;
//			cout << "please enter the begin address of the block\n>>";
//			cin >> paddr;
//			free(paddr);
//		}
//		else if (choice == 3) show();	//显示主存
//		else if (choice == 0) break;	//退出
//		else							//输入操作有误
//		{
//			cout << "incorrect input，please try again ！" << endl;
//			continue;
//		}
//	}
//}
