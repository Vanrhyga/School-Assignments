#include"processM_head.h"

//默认 PCB 创建函数
processControlBlock::processControlBlock() {
}

//带参 PCB 创建函数
processControlBlock::processControlBlock(string PID, string name, processType type) {
	this->PID = PID;
	this->name = name;
	this->type = type;
}

//子进程创建函数
void processControlBlock::createChildP(string PID, string name, processType type) {
	PCB child;
	child.PID = PID;
	child.name = name;
	child.parentPID = this->PID;
	child.type = type;
	this->childProcess.insert(make_pair(child.PID, child));				//建立所属关系
}

//进程删除函数
void processControlBlock::destroyProcess() {
	auto childP = childProcess.begin();
	auto process1 = find(readyList[0].begin(), readyList[0].end(), PID);
	auto process2 = find(readyList[1].begin(), readyList[1].end(), PID);
	auto process3 = find(blockedList[0].begin(), blockedList[0].end(), PID);
	auto process4 = find(blockedList[1].begin(), blockedList[1].end(), PID);
	while (childP != childProcess.end()) {								//删除子进程
		childProcess.erase(childP->second.PID);
		++childP;
	}
	switch (list){														//删除父进程
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

//所需资源增加函数
void processControlBlock::increaseResource(string RID, int amount) {
	if (resources.find(RID) != resources.end())
		(*resources.find(RID)).second += amount;
	else
		resources.insert(make_pair(RID, amount));

}

//资源统计函数
int processControlBlock::countResource(string RID) {
	if (resources.find(RID) != resources.end()) {
		int amount = resources.find(RID)->second;
		resources.erase(RID);
		return amount;
	}
	else
		return 0;
}

//资源创建函数
resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

//资源申请函数
int resource::request(string PID, int amount) {
	if (amount > this->amount) {										//若超出资源总量
		throw "Error! Requested resource exceeds upper limit!";
		return -1;
	}
	else {
		if (amount <= freeAmount) {										
			freeAmount -= amount;
			return 1;
		}
		else {															//若超出空闲资源数
			waitingL.push_back({ PID,amount });							//插入等待队列
			return 0;
		}
	}
}

//资源释放函数
string resource::release(int amount) {
	freeAmount += amount;
	if (!waitingL.empty()) {											//若等待队列非空
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

//队列初始化函数
void initList() {
	readyList[0].push_front("");
	readyList[1].push_front("");
}

PCB& getProcess(string PID) {
	return (*(process.find(PID))).second;
}

resource& getResource(string RID) {
	return (*(allResource.find(RID))).second;
}

void insertProcess(string PID, PCB p) {
	process.insert(make_pair(PID, p));
}

void outProcess(string PID) {
	PCB &p = getProcess(PID);
	map<string, PCB>& children = p.childProcess;
	auto child = children.begin();
	while (child != children.end()) {
		process.erase(child->second.PID);
		++child;
	}
	process.erase(PID);
}

//就绪队列插入函数
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
	PCB &p = getProcess(PID);
	p.state = ready;
	p.list = readyL;
}

//就绪队列移出函数
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

//阻塞队列插入函数
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
	PCB &p = getProcess(PID);
	p.state = blocked;
	p.list = blockedL;
}

//阻塞队列移出函数
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

//上下文切换函数
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

//运行状态切换函数
void intoRunning(processType type) {
	contextSwitch(type);
	switch (type){
	case processType::user:
		*(readyList[0].begin()) = *(++readyList[0].begin());
		readyList[0].erase(++readyList[0].begin());
		PCB &p1 = getProcess(*(readyList[0].begin()));
		p1.state = running;
		break;
	case processType::system:
		*(readyList[1].begin()) = *(++readyList[1].begin());
		readyList[1].erase(++readyList[1].begin());
		PCB &p2 = getProcess(*(readyList[1].begin()));
		p2.state = running;
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

//运行状态结束函数
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

//当前执行进程获取函数
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

//调度函数
void dispatcher() {
	if (getRunningProcess() == "") {
		if (readyList[1].size() > 1)
			intoRunning(processType::system);
		else if (readyList[0].size() > 1)
			intoRunning(processType::user);
	}
}

void RR() {
	PCB &p = (*(process.find(getRunningProcess()))).second;
	contextSwitch(p.type);
	insertRL(p.PID, p.type);
	dispatcher();
}