#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<assert.h>
#include<string>
#include<stdio.h>

#define Free "0"			//����״̬
#define Busy "1"			//����״̬
#define OK 1				//���
#define ERROR -1			//����
#define PGSIZE 1024			//ҳ��С4096
#define MSIZE 102400		//����ڴ�ռ�Ϊ100KB
#define VM_USED "-1"		//��������ڴ�ʱID�ţ�Ϊ�����̺������֣�
#define FIRST_FIT 1
//#define BEST_FIT 1
//#define WORST_FIT 1

using namespace std;

typedef size_t Status;

typedef struct freearea		//������˵����ṹ
{
	string ID;			    //ռ�õĽ��̺� ��������棬IDΪ-1
	size_t size;				//������С
	size_t address;		    //������ַ
	string state;		    //״̬
}ElemType;

//----------  ���Ա��˫������洢�ṹ  ------------
typedef struct memNode
{
	ElemType data;
	struct memNode *prior; //ǰ��ָ��
	struct memNode *next;  //���ָ��
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