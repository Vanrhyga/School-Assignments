#pragma once
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<ctime>
#include<sstream>
#include<Windows.h>
using namespace std;
/* 注：父子进程级联删除;
正在运行进程为就绪队列首元素
*/


/*常量*/
#define MAX_RESOURCE_AMOUNT 12
#define MAX_SIZE			12
#define MAX_PROCESS_AMOUNT  36
#define MAX_RUNTIME			24


/*枚举*/
enum processState {								//进程状态
	ready,running,blocked
};
enum listType {									//队列种类
	readyL,blockedL
};
enum processType {								//进程种类
	user,forSystem
};
enum processOperate {							//进程操作
	request,release,timeout,dispatch
};


/*结构*/
typedef struct processControlBlock {			//进程控制块
	string PID;									//进程标识
	string name;								//进程名称
	string parentPID = "";						//父进程名称
	int runtime = rand() % MAX_RUNTIME + 1;
	processState state = ready;					//进程状态
	processType type;							//进程种类
	listType list = readyL;						//所在队列
	map<string, int> resources;					//已占有资源
	map<string, processControlBlock> childProcess;						//子进程
	processControlBlock();
	processControlBlock(string PID, string name, processType type);
	void createChildP(string PID, string name, processType type);		//创建子进程
	void increaseResource(string RID, int amount);						//增加所需资源
	int countResource(string RID);				//统计资源
	void releaseAllResource(string *s);
}PCB;
struct processInfo {							//进程信息
	string PID;									//进程标识
	int reqAmount;								//申请资源数
};
struct resource{								//资源
	string RID;									//资源标识
	int amount;									//资源总量
	int freeAmount;								//空闲资源数
	vector<processInfo> waitingL;				//等待队列
	resource(string RID, int amount, int freeAmount);
	int request(string PID, int amount);		//申请资源
	string release(int amount);					//释放资源
};


/*全局变量*/
extern list<string> readyList[2];				//就绪队列
extern list<string> blockedList[2];				//阻塞队列
extern map<string, PCB> process;
extern map<string, resource> allResource;
extern int counter;
extern int timeSlot;


/*功能函数*/
void initList();								//初始化队列
PCB& getProcess(string PID);
resource& getResource(string RID);
void insertProcess(string PID, PCB p);
void outProcess(string PID);
void destroyProcess(string PID);				//删除进程
void killProcess(string PID);
void insertRL(string PID, processType type);	//插入就绪队列
void outRL(processType type);					//移出就绪队列
void insertBL(string PID, processType type);	//插入阻塞队列
void outBL(string PID, processType type);		//移出阻塞队列
void contextSwitch(processType type);			//上下文切换
void intoRunning(processType type);				//切换运行状态
void outOfRunning(string PID, processType type, processState state);	//结束运行状态
string getRunningProcess();						//获取当前执行进程
void dispatcher();								//调度
void RR();


/*工具函数*/
string toString(int i);
string nametoPID(string name);
DWORD WINAPI TIMER_SEC(LPVOID lpparentet);
DWORD WINAPI TIMER_FIVE_SEC(LPVOID lpparentet);