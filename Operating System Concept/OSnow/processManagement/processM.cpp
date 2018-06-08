#include "processM_head.h"

list<string> readyList[2];
list<string> blockedList[2];
map<string, PCB> process;
map<string, resource> allResource;
ofstream ofp;

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
	child.PID = PID;														//�ӽ������Գ�ʼ��
	child.name = name;
	child.parentPID = this->PID;
	child.type = type;
	child.size = this->size;
	if (!insertProcess(child.PID, child)) {									//�������ɹ�
		this->childProcess.insert(make_pair(child.PID, child));				//����������ϵ
		insertRL(child.PID, child.type);									//�����������
	}
	else {																	//����
		recordTime();														//������־��¼��Ӧ��Ϣ
		ofp << "�ռ䲻�㣬�ӽ��̴���ʧ��" << endl;
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

//���ӽ�����Դ�ͷź���
void processControlBlock::releaseAllResource(string* s) {
	int i;
	auto iter = childProcess.begin();
	while (iter != childProcess.end()) {								//�ͷ��ӽ�����Դ
		PCB &p = iter->second;
		p.releaseAllResource(s);
		iter++;
	}
	auto riter = resources.begin();
	if (riter != resources.end()) {
		ofp << "������Դ�ͷ�" << endl;
		ofp << "�������ƣ�" << name << "  ";
		ofp << "���̱�ʶ��" << PID << "  ";
		if (type == processType::forSystem)
			ofp << "�������ࣺϵͳ����" << endl;
		else
			ofp << "�������ࣺ�û�����" << endl;
	}
	for (i = 0; riter != resources.end(); riter++, i++) {				//ѭ���ͷ�������Դ
		resource &r = getResource(riter->first);
		ofp << "��Դ���ࣺR" << r.RID << "  ";
		ofp << "��Դ������" << riter->second << endl;
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
	if (amount <= freeAmount) {											//��δ����������Դ��
		freeAmount -= amount;											//������Դ��������Ӧ����
		return 1;
	}
	else {																//����
		waitingL.push_back({ PID,amount });								//����ȴ�����
		return 0;
	}
}

//��Դ�ͷź���
string resource::release(int amount, string PID) {
	freeAmount += amount;												//������Դ��������Ӧ����
	if (!waitingL.empty()) {											//���ȴ����зǿ�
		auto &nextProcess = *(waitingL.begin());						//ȡ����Ԫ��
		string s = nextProcess.PID;
		if (nextProcess.PID != PID) {
			waitingL.erase(waitingL.begin());							//�������
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

int insertProcess(string PID, PCB p) {
	vaddr addr;
	/*if (p.parentPID != "")
		addr = fork_memory(p.parentPID, PID);
	else*/
		addr = Allocate_VM(PID, p.size);
	if (addr) {
		p.start = addr;
		recordTime();
		ofp << "���̴���" << endl;
		ofp << "�������ƣ�" << p.name << "  ";
		ofp << "���̱�ʶ��" << p.PID << "  ";
		if (p.type == processType::forSystem)
			ofp << "�������ࣺϵͳ����" << "  ";
		else
			ofp << "�������ࣺ�û�����" << "  ";
		ofp << "����ʱ�䣺" << p.runtime << endl;
		process.insert(make_pair(PID, p));
		return 0;
	}
	else
		return 1;
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
	bool tmp = getRunningProcess() == PID;								//�ж���ɾ�������Ƿ�Ϊ����ִ�н���
	PCB &p = getProcess(PID);										
	i = p.childProcess.size();
	while (i > 0) {														//���ӽ��̼���ɾ��
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
	if (p.state == blocked) {
		auto riter = allResource.begin();
		while (riter != allResource.end()) {
			for (auto waitingIter = riter->second.waitingL.begin(); waitingIter < riter->second.waitingL.end(); waitingIter++)
				if (waitingIter->PID == p.PID) {
					riter->second.waitingL.erase(waitingIter);
					break;
				}
			riter++;
		}
	}
	if (tmp)															//����ɾ������Ϊ����ִ�н���
		contextSwitch(p.type);											//�������л�
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
	ofp << "�������ƣ�" << p.name << "  ";
	ofp << "���̱�ʶ��" << p.PID << "  ";
	if (p.type == processType::forSystem)
		ofp << "�������ࣺϵͳ����" << endl;
	else
		ofp << "�������ࣺ�û�����" << endl;
	process.erase(PID);
	Free_VM(PID);														//�ͷŽ�����ռ�ռ�
	if (tmp)															//����ɾ������Ϊ����ִ�н���
		dispatcher();													//����
}

void killProcess(string PID) {
	int i;
	PCB &tmp = getProcess(PID);
	string nextPList[MAX_RESOURCE_AMOUNT];
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
		nextPList[i] = "";
	tmp.releaseAllResource(nextPList);
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
		if (nextPList[i] != "") {
			ofp << "�����������" << endl;
			break;
		}
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		if (nextPList[i] != "") {
			PCB &nextProcess = (*(process.find(nextPList[i]))).second;
			ofp << "�������ƣ�" << nextProcess.name << "  ";
			ofp << "���̱�ʶ��" << nextProcess.PID << "  ";
			if (nextProcess.type == processType::forSystem)
				ofp << "�������ࣺϵͳ����" << endl;
			else
				ofp << "�������ࣺ�û�����" << endl;
			outBL(nextProcess.PID, nextProcess.type);
			insertRL(nextProcess.PID, nextProcess.type);
		}
	}
	ofp << "����ɾ��" << endl;
	destroyProcess(PID);
}

//�������в��뺯��
void insertRL(string PID, processType type) {
	switch (type) {														//���ݽ������࣬������Ӧ����
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
	p.state = ready;													//���Ľ�������
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
	switch (type) {
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
	switch (type) {
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
	switch (state) {
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
	if (getRunningProcess() == "") {									//��������ִ�н���
		if (readyList[1].size() > 1)									//��ϵͳ���̵ȴ����зǿ�
			intoRunning(processType::forSystem);						//ȡ����Ԫ��ִ��
		else if (readyList[0].size() > 1)								//�������û����̵ȴ����зǿ�
			intoRunning(processType::user);								//ȡ����Ԫ��ִ��
	}
}

//ʱ��Ƭ��ת����
void RR() {
	string s = getRunningProcess();
	if (s == "")														//��������ִ�н���
		return;
	PCB &p = (*(process.find(s))).second;	
	contextSwitch(p.type);												//�����������л�
	insertRL(p.PID, p.type);											//�ý���תΪ����״̬
	dispatcher();														//����
}

void annotation() {
	ofp.open("processM_log.txt", ios::out);
	ofp << "**************************************************************************************" << endl;
	ofp << ">>*                                                                  ������־                                                                   *<<" << endl;
	ofp << "**************************************************************************************" << endl;
}

void recordTime() {
	SYSTEMTIME time;
	GetSystemTime(&time);
	ofp << time.wYear << "-" << time.wMonth << "-" << time.wDay << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " ";
}

void pri() {
	unsigned int i;
	cout << "Running process:" << endl;
	auto iter = process.begin();
	while (iter != process.end()) {
		if (iter->second.state == running) {
			cout << iter->second.name << "::" << iter->first << "::";
			if (iter->second.type == user)
				cout << "user" << endl;
			else
				cout << "system" << endl;
			cout << "Runtime:" << iter->second.runtime << endl;
		}
		iter++;
	}
	if (readyList[0].size() > 1 || readyList[1].size() > 1) {
		cout << endl << "Ready processes:" << endl;
		for (iter = process.begin(); iter != process.end(); iter++)
			if (iter->second.type == processType::forSystem&&iter->second.state == ready) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.type == user)
					cout << "user" << endl;
				else
					cout << "system" << endl;
			}
		for (iter = process.begin(); iter != process.end(); iter++)
			if (iter->second.type == processType::user&&iter->second.state == ready) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.type == user)
					cout << "user" << endl;
				else
					cout << "system" << endl;
			}
	}
	if (blockedList[0].size() > 0 || blockedList[1].size() > 0) {
		cout << endl << "Blocked processes:" << endl;
		for (iter = process.begin(); iter != process.end(); iter++)
			if (iter->second.type == processType::forSystem&&iter->second.state == blocked) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.type == user)
					cout << "user" << endl;
				else
					cout << "system" << endl;
			}
		for (iter = process.begin(); iter != process.end(); iter++)
			if (iter->second.type == processType::user&&iter->second.state == blocked) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.type == user)
					cout << "user" << endl;
				else
					cout << "system" << endl;
			}
	}
	cout << endl << "Resources:" << endl;
	auto iter1 = allResource.begin();
	while (iter1 != allResource.end()) {
		cout << "R" << iter1->second.RID << "::" << iter1->second.waitingL.size() << "\t";
		for (i = 0; i < iter1->second.waitingL.size(); i++)
			cout << process.find(iter1->second.waitingL[i].PID)->second.name + "::" + iter1->second.waitingL[i].PID + " ";
		cout << endl;
		iter1++;
	}
}

string toString(int i) {
	stringstream s;
	s << i;
	return s.str();
}

void painting() {
	cout << "                       ::" << endl << "                      :;J7, :,                        ::;7:" << endl;
	cout << "                      ,ivYi, ,                       ;LLLFS:" << endl << "                      :iv7Yi                       :7ri;j5PL" << endl;
	cout << "                     ,:ivYLvr                    ,ivrrirrY2X," << endl << "                     :;r@Wwz.7r:                :ivu@kexianli." << endl;
	cout << "                    :iL7::,:::iiirii:ii;::::,,irvF7rvvLujL7ur" << endl << "                   ri::,:,::i:iiiiiii:i:irrv177JX7rYXqZEkvv17" << endl;
	cout << "                ;i:, , ::::iirrririi:i:::iiir2XXvii;L8OGJr71i" << endl << "              :,, ,,:   ,::ir@mingyi.irii:i:::j1jri7ZBOS7ivv," << endl;
	cout << "                 ,::,    ::rv77iiiriii:iii:i::,rvLq@huhao.Li" << endl << "             ,,      ,, ,:ir7ir::,:::i;ir:::i:i::rSGGYri712:" << endl;
	cout << "           :::  ,v7r:: ::rrv77:, ,, ,:i7rrii:::::, ir7ri7Lri" << endl << "          ,     2OBBOi,iiir;r::        ,irriiii::,, ,iv7Luur:" << endl;
	cout << "        ,,     i78MBBi,:,:::,:,  :7FSL: ,iriii:::i::,,:rLqXv::" << endl << "        :      iuMMP: :,:::,:ii;2GY7OBB0viiii:i:iii:i:::iJqL;::" << endl;
	cout << "       ,     ::::i   ,,,,, ::LuBBu BBBBBErii:i:i:i:i:i:i:r77ii" << endl << "      ,       :       , ,,:::rruBZ1MBBqi, :,,,:::,::::::iiriri:" << endl;
	cout << "     ,               ,,,,::::i:  @arqiao.       ,:,, ,:::ii;i7:" << endl << "    :,       rjujLYLi   ,,:::::,:::::::::,,   ,:i,:,,,,,::i:iii" << endl;
	cout << "    ::      BBBBBBBBB0,    ,,::: , ,:::::: ,      ,,,, ,,:::::::" << endl << "    i,  ,  ,8BMMBBBBBBi     ,,:,,     ,,, , ,   , , , :,::ii::i::" << endl;
	cout << "    :      iZMOMOMBBM2::::::::::,,,,     ,,,,,,:,,,::::i:irr:i:::," << endl << "    i   ,,:;u0MBMOG1L:::i::::::  ,,,::,   ,,, ::::::i:i:iirii:i:i:" << endl;
	cout << "    :    ,iuUuuXUkFu7i:iii:i:::, :,:,: ::::::::i:i:::::iirr7iiri::" << endl << "    :     :rk@Yizero.i:::::, ,:ii:::::::i:::::i::,::::iirrriiiri::," << endl;
	cout << "     :      5BMBBBBBBSr:,::rv2kuii:::iii::,:i:,, , ,,:,:i@petermu.," << endl << "          , :r50EZ8MBBBBGOBBBZP7::::i::,:::::,: :,:,::i;rrririiii::" << endl;
	cout << "              :jujYY7LS0ujJL7r::,::i::,::::::::::::::iirirrrrrrr:ii:" << endl << "           ,:  :@kevensun.:,:,,,::::i:i:::::,,::::::iir;ii;7v77;ii;i," << endl;
	cout << "           ,,,     ,,:,::::::i:iiiii:i::::,, ::::iiiir@xingjief.r;7:i," << endl << "        , , ,,,:,,::::::::iiiiiiiiii:,:,:::::::::iiir;ri7vL77rrirri::" << endl;
	cout << "         :,, , ::::::::i:::i:::i:i::,,,,,:,::i:i:::iir;@Secbone.ii:::" << endl;
	cout << ">>********************************************************************************<<" << endl;
	cout << ">>*                              Welcome to OSnow!                               *<<" << endl;
	cout << ">>********************************************************************************<<" << endl << endl;
}