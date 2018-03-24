#include"processM_head.h"

processControlBlock::processControlBlock() {
}

processControlBlock::processControlBlock(string PID, string name, processType type) {
	this->PID = PID;
	this->name = name;
	this->type = type;
}

void processControlBlock::createChildP(string PID, string name, string parentPID, processType type) {
	PCB child;
	child.PID = PID;
	child.name = name;
	child.parentPID = parentPID;
	child.type = type;
	this->childProcess.insert(make_pair(child.PID, child));
}

void processControlBlock::destroyProcess() {
	auto childP = childProcess.begin();
	auto process1 = find(readyList[0].begin(), readyList[0].end(), PID);
	auto process2 = find(readyList[1].begin(), readyList[1].end(), PID);
	auto process3 = find(blockedList[0].begin(), blockedList[0].end(), PID);
	auto process4 = find(blockedList[1].begin(), blockedList[1].end(), PID);
	while (childP != childProcess.end()) {
		childProcess.erase(childP->second.PID);
		++childP;
	}
	switch (list){
	case readyL:
		switch (type){
		case processType::user:
			readyList[0].erase(process1);
		case processType::system:
			readyList[1].erase(process2);
		default:
			throw "Type error! Invalid type value!";
			break;
		}
	case blockedL:
		switch (type){
		case processType::user:
			blockedList[0].erase(process3);
			break;
		case processType::system:
			blockedList[1].erase(process4);
			break;
		default:
			throw "Type error! Invalid type value!";
			break;
		}
	default:
		throw "List error! Invalid list value!";
		break;
	}
}

void processControlBlock::changeState(processOperate operate) {
	switch (operate){
	case request:
		if (state == running) {
			state = blocked;
			list = blockedL;
		}
		else
			throw "State error!";
		break;
	case release:
		if (state == blocked) {
			state = ready;
			list = readyL;
		}
		else
			throw "State error!";
		break;
	case timeout:
		if (state == running) {
			state = ready;
			list = readyL;
		}
		else
			throw "State error!";
		break;
	case dispatch:
		if (state == ready)
			state = running;
		else
			throw "State error!";
	default:
		throw "Operation error!";
		break;
	}
}

void processControlBlock::getResource(string RID, int amount) {
	if (resources.find(RID) != resources.end())
		(*resources.find(RID)).second += amount;
	else
		resources.insert(make_pair(RID, amount));

}

int processControlBlock::releaseResource(string RID) {
	if (resources.find(RID) != resources.end()) {
		int amount = resources.find(RID)->second;
		resources.erase(RID);
		return amount;
	}
	else
		return 0;
}

resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

int resource::request(string PID, int amount) {
	if (amount > this->amount) {
		throw "Error! Requested resource exceeds upper limit!";
		return -1;
	}
	else {
		if (amount <= freeAmount) {
			freeAmount -= amount;
			return 1;
		}
		else {
			waitingL.push_back({ PID,amount });
			return 0;
		}
	}
}

string resource::release(string PID, int amount) {
	freeAmount += amount;
	if (!waitingL.empty()) {
		auto &nextProcess = *(waitingL.begin());
		if (nextProcess.reqAmount <= freeAmount) {
			freeAmount -= nextProcess.reqAmount;
			waitingL.erase(waitingL.begin());
			return nextProcess.PID;
		}
		else
			return "";
	}
	else
		return "";
}

void initList() {
	readyList[0].push_front("");
	readyList[1].push_front("");
}

void insertRL(string PID, processType type) {
	switch (type){
	case processType::user:
		readyList[0].push_back(PID);
		break;
	case processType::system:
		readyList[1].push_back(PID);
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void outRL(string PID, processType type) {
	switch (type) {
	case processType::user:
		readyList[0].erase(++readyList[0].begin());
		break;
	case processType::system:
		readyList[1].erase(++readyList[1].begin());
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void insertBL(string PID, processType type) {
	switch (type) {
	case processType::user:
		blockedList[0].push_back(PID);
		break;
	case processType::system:
		blockedList[1].push_back(PID);
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void outBL(string PID, processType type) {
	switch (type) {
	case processType::user:
		blockedList[0].pop_front();
		break;
	case processType::system:
		blockedList[1].pop_front();
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void contextSwitch(processType type) {
	switch (type){
	case processType::user:
		*(readyList[0].begin()) = "";
		break;
	case processType::system:
		*(readyList[1].begin()) = "";
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void intoRunning(processType type) {
	contextSwitch(type);
	switch (type){
	case processType::user:
		*(readyList[0].begin()) = *(++readyList[0].begin());
		readyList[0].erase(++readyList[0].begin());
		break;
	case processType::system:
		*(readyList[1].begin()) = *(++readyList[1].begin());
		readyList[1].erase(++readyList[1].begin());
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

void outOfRunning(string PID, processType type, processState state) {
	contextSwitch(type);
	switch (state){
	case ready:
		insertRL(PID, type);
		break;
	case blocked:
		insertBL(PID, type);
		break;
	default:
		throw "State error!";
		break;
	}
}

string getRunningProcess() {
	string PID;
	if (*(readyList[1].begin()) != "")
		PID = *(readyList[1].begin());
	else if (*(readyList[0].begin()) != "")
		PID = *(readyList[0].begin());
	else
		PID = "";
	return PID;
}

void dispatcher() {
	if (getRunningProcess() == "") {
		if (readyList[1].size() > 1)
			intoRunning(processType::system);
		else if (readyList[0].size() > 1)
			intoRunning(processType::user);
	}
}