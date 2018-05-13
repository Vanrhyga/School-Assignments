#pragma once
#define Free 0			//空闲状态
#define Busy 1			//已用状态
#define OK 1			//完成
#define ERROR -1		//出错
#define PGSIZE 4096		//页大小4096
#define MSIZE 1024*1024 //最大内存空间为1MB
#define VM_USED -1		//虚存申请内存时ID号（为跟进程号相区分）

#define FIRST_FIT 1
//#define BEST_FIT 1
//#define WORST_FIT 1

extern size_t   memNum ;//物存总大小
extern time_t   now;           //时间
extern char*    dt;			//输出时间字符

typedef size_t Status;

typedef struct freearea//定义一个空闲区说明表结构
{
	int ID;			   //占用的进程号 若分配虚存，ID为-1
	long size;		   //分区大小
	long address;	   //分区地址
	int state;		   //状态
}ElemType;

//----------  线性表的双向链表存储结构  ------------
typedef struct memNode //double linked list
{
	ElemType data;
	struct memNode *prior; //前趋指针
	struct memNode *next;  //后继指针
}memNode, *memList;

extern memList block_first;		//头结点
extern memList block_last;			//尾结点

/*@para:int ID(-1 for VM),size_t request
  @return:size_t paddr OR ERROR(-1)*/
Status alloc(int,size_t);

/*@para:size_t paddr
  @return:OK(1) OR ERROR(-1)*/
Status free(size_t);	

/*@para:int ID(-1 for VM),size_t request
  @return:size_t paddr OR ERROR(-1)*/
Status First_fit(int, size_t);  
Status Best_fit(int, size_t);  
Status Worst_fit(int, size_t);  

void show();				    //查看分配
Status Initblock();				//初始化内存空间

char read_pm(size_t paddr);
int write_pm(size_t paddr, char c);
