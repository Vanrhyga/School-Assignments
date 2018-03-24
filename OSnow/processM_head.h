#pragma once
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<sstream>
using namespace std;


enum processState {
	ready,running,blocked
};
enum listType {
	readyL,blockedL
};
enum processType {
	user,system
};
enum processOperate {
	request,release,timeout,dispatch
};


list<string> readyList[2];
list<string> blockedList[2];
map<string, PCB> process;
map<string, resource> allResource;


typedef struct processControlBlock {
	string PID;
	string name;
	string parentPID = "";
	processState state = ready;
	processType type;
	listType list = readyL;
	map<string, int> resources;
	map<string, processControlBlock> childProcess;
	processControlBlock();
	processControlBlock(string PID, string name, processType type);
	void createChildP(string PID, string name, string parentPID, processType type);
	void destroyProcess();
	void changeState(processOperate operate);
	void getResource(string RID, int amount);
	int releaseResource(string RID);
}PCB;
struct processInfo {
	string PID;
	int reqAmount;
};
struct resource{
	string RID;
	int amount;
	int freeAmount;
	vector<processInfo> waitingL;
	resource(string RID, int amount, int freeAmount);
	int request(string PID, int amount);
	string release(string PID, int amount);
};


void initList();
void insertRL(string PID, processType type);
void outRL(string PID, processType type);
void insertBL(string PID, processType type);
void outBL(string PID, processType type);
void contextSwitch(processType type);
void intoRunning(processType type);
void outOfRunning(string PID, processType type, processState state);
//void intoBlocked(string PID, processType type);
string getRunningProcess();
void dispatcher();
//void RR();

string toString(int i) {
	stringstream s;
	s << i;
	return s.str();
}

string toRID(string name) {
	string RID;
	if (name == "R1")
		RID = "1";
	else if (name == "R2")
		RID = "2";
	else if (name == "R3")
		RID = "3";
	else if (name == "R4")
		RID = "4";
	else if (name == "R5")
		RID = "5";
	else
		throw "Error! Resource does not exist!";
	return RID;
}