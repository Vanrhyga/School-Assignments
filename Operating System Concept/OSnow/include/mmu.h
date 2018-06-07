#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<assert.h>
#include<string>
#include<stdio.h>

#define Free "0"			//空闲状态
#define Busy "1"			//已用状态
#define OK 1				//完成
#define ERROR -1			//出错
#define PGSIZE 1024			//页大小4096
#define MSIZE 102400		//最大内存空间为100KB
#define VM_USED "-1"		//虚存申请内存时ID号（为跟进程号相区分）
#define FIRST_FIT 1
//#define BEST_FIT 1
//#define WORST_FIT 1

using namespace std;

typedef size_t Status;

typedef struct freearea		//空闲区说明表结构
{
	string ID;			    //占用的进程号 若分配虚存，ID为-1
	size_t size;				//分区大小
	size_t address;		    //分区地址
	string state;		    //状态
}ElemType;

//----------  线性表的双向链表存储结构  ------------
typedef struct memNode
{
	ElemType data;
	struct memNode *prior; //前趋指针
	struct memNode *next;  //后继指针
}memNode, *memList;

Status Initblock();

/*@para:int ID(-1 for VM),size_t request
@return:size_t paddr OR ERROR(-1)*/
Status alloc(string, size_t);

/*@para:size_t paddr
@return:OK(1) OR ERROR(-1)*/
Status free(string);

/*@para:int ID(-1 for VM),size_t request
@return:size_t paddr OR ERROR(-1)*/
Status First_fit(string, size_t);
Status Best_fit(string, size_t);
Status Worst_fit(string, size_t);

/*@para: size_t paddr
@return: char character*/
char read_pm(size_t paddr);

/*@para:size_t paddr,char c
@return int*/
int write_pm(size_t paddr, char c);

void show();