#pragma once
#include<string>
#include<map>
using namespace std;

enum processState {
	init,ready,running,blocked
};
enum listType {
	readyL,runningL,blockedL
};
enum processType {
	user,system
};
enum processOperate {
	create,destroy,request,release,timeout,dispatch
};

typedef struct processControlBlock {
	
}PCB;