#include"processM_head.h"

//Ĭ�� PCB ��������
processControlBlock::processControlBlock() {
}

//���� PCB ��������
processControlBlock::processControlBlock(string PID, string name, processType type) {
	this->PID = PID;
	this->name = name;
	this->type = type;
}

//�ӽ��̴�������
void processControlBlock::createChildP(string PID, string name, processType type) {
	PCB child;
	child.PID = PID;
	child.name = name;
	child.parentPID = this->PID;
	child.type = type;
	this->childProcess.insert(make_pair(child.PID, child));				//����������ϵ
}

//����ɾ������
void processControlBlock::destroyProcess() {
	auto childP = childProcess.begin();
	auto process1 = find(readyList[0].begin(), readyList[0].end(), PID);
	auto process2 = find(readyList[1].begin(), readyList[1].end(), PID);
	auto process3 = find(blockedList[0].begin(), blockedList[0].end(), PID);
	auto process4 = find(blockedList[1].begin(), blockedList[1].end(), PID);
	while (childP != childProcess.end()) {								//ɾ���ӽ���
		childProcess.erase(childP->second.PID);
		++childP;
	}
	switch (list){														//ɾ��������
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

//������Դ���Ӻ���
void processControlBlock::increaseResource(string RID, int amount) {
	if (resources.find(RID) != resources.end())
		(*resources.find(RID)).second += amount;
	else
		resources.insert(make_pair(RID, amount));

}

//��Դͳ�ƺ���
int processControlBlock::countResource(string RID) {
	if (resources.find(RID) != resources.end()) {
		int amount = resources.find(RID)->second;
		resources.erase(RID);
		return amount;
	}
	else
		return 0;
}

//��Դ��������
resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

//��Դ���뺯��
int resource::request(string PID, int amount) {
	if (amount > this->amount) {										//��������Դ����
		throw "Error! Requested resource exceeds upper limit!";
		return -1;
	}
	else {
		if (amount <= freeAmount) {										
			freeAmount -= amount;
			return 1;
		}
		else {															//������������Դ��
			waitingL.push_back({ PID,amount });							//����ȴ�����
			return 0;
		}
	}
}

//��Դ�ͷź���
string resource::release(int amount) {
	freeAmount += amount;
	if (!waitingL.empty()) {											//���ȴ����зǿ�
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

//���г�ʼ������
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

//�������в��뺯��
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

//���������Ƴ�����
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

//�������в��뺯��
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

//���������Ƴ�����
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

//�������л�����
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

//����״̬�л�����
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

//����״̬��������
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

//��ǰִ�н��̻�ȡ����
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

//���Ⱥ���
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