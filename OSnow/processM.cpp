#include"processM_head.h"

list<string> readyList[2];
list<string> blockedList[2];
map<string, PCB> process;
map<string, resource> allResource;

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
	process.insert(make_pair(child.PID, child));
	insertRL(child.PID, child.type);
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

void processControlBlock::releaseAllResource(string* s) {
	int i;
	auto iter = childProcess.begin();
	while (iter != childProcess.end()) {
		PCB &p = iter->second;
		p.releaseAllResource(s);
		iter++;
	}
	auto riter = resources.begin();
	for (i = 0; riter != resources.end(); riter++, i++) {
		resource &r = getResource(riter->first);
		s[i] = r.release(riter->second, PID);
	}
	resources.clear();
}

//��Դ��������
resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

//��Դ���뺯��
int resource::request(string PID, int amount) {
	if (amount <= freeAmount) {										
		freeAmount -= amount;
		return 1;
	}
	else {															//������������Դ��
		waitingL.push_back({ PID,amount });							//����ȴ�����
		return 0;
	}
}

//��Դ�ͷź���
string resource::release(int amount, string PID) {
	freeAmount += amount;
	if (!waitingL.empty()) {											//���ȴ����зǿ�
		auto &nextProcess = *(waitingL.begin());
		string s = nextProcess.PID;
		if (nextProcess.PID != PID) {
			waitingL.erase(waitingL.begin());
			return s;
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

//����ɾ������
void destroyProcess(string PID) {
	int i = 0;
	bool tmp = getRunningProcess() == PID;
	PCB &p = getProcess(PID);
	i = p.childProcess.size();
	while (i > 0) {
		destroyProcess(p.childProcess.begin()->first);
		i--;
	}
	if (p.type == processType::user) {
		if (p.state == ready) {
			auto iter1 = readyList[0].begin();
			while (iter1 != readyList[0].end()) {
				if (*iter1 == PID)
					break;
				iter1++;
			}
			readyList[0].erase(iter1);
		}
		else if (p.state == blocked) {
			auto iter2 = blockedList[0].begin();
			while (iter2 != blockedList[0].end()) {
				if (*iter2 == PID)
					break;
				iter2++;
			}
			blockedList[0].erase(iter2);
		}
	}
	else {
		if (p.state == ready) {
			auto iter3 = readyList[1].begin();
			while (iter3 != readyList[1].end()) {
				if (*iter3 == PID)
					break;
				iter3++;
			}
			readyList[1].erase(iter3);
		}
		else if (p.state == blocked) {
			auto iter4 = blockedList[1].begin();
			while (iter4 != blockedList[1].end()) {
				if (*iter4 == PID)
					break;
				iter4++;
			}
			blockedList[1].erase(iter4);
		}
	}
	if (tmp)
		contextSwitch(p.type);
	if (process.find(p.parentPID) != process.end()) {
		map<string, PCB>& children = process.find(p.parentPID)->second.childProcess;
		auto iter5 = children.begin();
		while (iter5 != children.end()) {
			if (iter5->first == PID) {
				children.erase(PID);
				break;
			}
			iter5++;
		}
	}
	process.erase(PID);
	if (tmp)
		dispatcher();
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
	destroyProcess(PID);
}

//�������в��뺯��
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

//���������Ƴ�����
void outRL(processType type) {
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

//�������в��뺯��
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

//���������Ƴ�����
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

//�������л�����
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

//����״̬�л�����
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

//����״̬��������
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
		nextPList[i] = r.release(riter->second, PID);
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
			intoRunning(processType::forSystem);
		else if (readyList[0].size() > 1) 
			intoRunning(processType::user);
	}
}

void RR() {
	string s = getRunningProcess();
	if (s == "")
		return;
	PCB &p = (*(process.find(s))).second;
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