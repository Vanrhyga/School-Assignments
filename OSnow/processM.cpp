#include"processM_head.h"

list<string> readyList[2];
list<string> blockedList[2];
map<string, PCB> process;
map<string, resource> allResource;

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
	process.insert(make_pair(child.PID, child));
	insertRL(child.PID, child.type);
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

void processControlBlock::releaseAllResource(string* s) {
	int i;
	auto iter = childProcess.begin();
	while (iter != childProcess.end()) {
		PCB &p = iter->second;
		p.releaseAllResource(s);
		iter++;
	}
	auto riter = resources.cbegin();
	for (i = 0; riter != resources.cend();i++) {
		resource &r = getResource(riter->first);
		s[i] = r.release(riter->second);
	}
}

//资源创建函数
resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

//资源申请函数
int resource::request(string PID, int amount) {
	if (amount > this->amount) 										//若超出资源总量
		return -1;
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
	processType type;
	if (rand() % 2)
		type = processType::forSystem;
	else
		type = processType::user;
	insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type));
	insertRL(toString(counter), type);
	dispatcher();
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
		process.erase(child->first);
		++child;
	}
	process.erase(PID);
}

//进程删除函数
void destroyProcess(string PID) {
	PCB &p = getProcess(PID);
	auto iter = p.childProcess.begin();
	while (iter != p.childProcess.end()) {
		process.erase(iter->first);
		iter++;
	}
	process.erase(PID);
}

void killProcess(string PID) {
	int i;
	PCB &tmp = getProcess(PID);
	string nextPList[MAX_RESOURCE_AMOUNT];
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
		nextPList[i] = "";
	tmp.releaseAllResource(nextPList);
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		if (nextPList[i] != "") {
			PCB &nextProcess = (*(process.find(nextPList[i]))).second;
			outBL(nextProcess.PID, nextProcess.type);
			insertRL(nextProcess.PID, nextProcess.type);
		}
	}
	auto iter = tmp.childProcess.begin();
	while (iter != tmp.childProcess.end()) {
		destroyProcess(iter->first);
		iter++;
	}
	if (getRunningProcess() == tmp.PID) {
		contextSwitch((*(process.find(getRunningProcess()))).second.type);
		dispatcher();
	}
	destroyProcess(tmp.PID);
}

//就绪队列插入函数
void insertRL(string PID, processType type) {
	switch (type){
	case processType::user:
		readyList[0].push_back(PID);
		break;
	case processType::forSystem:
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
	case processType::forSystem:
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
	case processType::forSystem:
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
	case processType::forSystem:
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
	case processType::forSystem:
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
		getProcess(*(readyList[0].begin())).state = running;
		break;
	case processType::forSystem:
		*(readyList[1].begin()) = *(++readyList[1].begin());
		readyList[1].erase(++readyList[1].begin());
		getProcess(*(readyList[1].begin())).state = running;
		break;
	default:
		throw "Type error! Invalid type value!";
		break;
	}
}

//运行状态结束函数
void outOfRunning(string PID, processType type, processState state) {
	int i;
	string nextPList[MAX_RESOURCE_AMOUNT];
	contextSwitch(type);
	PCB &p = (*(process.find(PID))).second;
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
		nextPList[i] = "";
	auto riter = p.resources.cbegin();
	for (i = 0; riter != p.resources.cend(); i++) {
		resource &r = getResource(riter->first);
		nextPList[i] = r.release(riter->second);
	}
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		if (nextPList[i] != "") {
			PCB &nextProcess = (*(process.find(nextPList[i]))).second;
			outBL(nextProcess.PID, nextProcess.type);
			insertRL(nextProcess.PID, nextProcess.type);
		}
	}
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
			intoRunning(processType::forSystem);
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

string toString(int i) {
	stringstream s;
	s << i;
	return s.str();
}

string nametoPID(string name) {
	auto iter = process.begin();
	while (iter != process.end()) {
		if (iter->second.name == name)
			return iter->second.PID;
		iter++;
	}
	return "";
}