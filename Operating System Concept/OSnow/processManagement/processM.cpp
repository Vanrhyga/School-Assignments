#include "processM_head.h"

list<string> readyList[2];
list<string> blockedList[2];
map<string, PCB> process;
map<string, resource> allResource;
ofstream ofp;

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
	child.PID = PID;														//子进程属性初始化
	child.name = name;
	child.parentPID = this->PID;
	child.type = type;
	child.size = this->size;
	if (!insertProcess(child.PID, child)) {									//若创建成功
		this->childProcess.insert(make_pair(child.PID, child));				//建立所属关系
		insertRL(child.PID, child.type);									//进入就绪队列
	}
	else {																	//否则
		recordTime();														//进程日志记录相应信息
		ofp << "空间不足，子进程创建失败" << endl;
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

//父子进程资源释放函数
void processControlBlock::releaseAllResource(string* s) {
	int i;
	auto iter = childProcess.begin();
	while (iter != childProcess.end()) {								//释放子进程资源
		PCB &p = iter->second;
		p.releaseAllResource(s);
		iter++;
	}
	auto riter = resources.begin();
	if (riter != resources.end()) {
		ofp << "进程资源释放" << endl;
		ofp << "进程名称：" << name << "  ";
		ofp << "进程标识：" << PID << "  ";
		if (type == processType::forSystem)
			ofp << "进程种类：系统进程" << endl;
		else
			ofp << "进程种类：用户进程" << endl;
	}
	for (i = 0; riter != resources.end(); riter++, i++) {				//循环释放所有资源
		resource &r = getResource(riter->first);
		ofp << "资源种类：R" << r.RID << "  ";
		ofp << "资源数量：" << riter->second << endl;
		s[i] = r.release(riter->second, PID);
	}
	resources.clear();
}

//资源创建函数
resource::resource(string RID, int amount, int freeAmount) {
	this->RID = RID;
	this->amount = amount;
	this->freeAmount = freeAmount;
}

//资源申请函数
int resource::request(string PID, int amount) {
	if (amount <= freeAmount) {											//若未超出空闲资源数
		freeAmount -= amount;											//空闲资源数减少相应数量
		return 1;
	}
	else {																//否则
		waitingL.push_back({ PID,amount });								//插入等待队列
		return 0;
	}
}

//资源释放函数
string resource::release(int amount, string PID) {
	freeAmount += amount;												//空闲资源数增加相应数量
	if (!waitingL.empty()) {											//若等待队列非空
		auto &nextProcess = *(waitingL.begin());						//取其首元素
		string s = nextProcess.PID;
		if (nextProcess.PID != PID) {
			waitingL.erase(waitingL.begin());							//解除阻塞
			return s;
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

int insertProcess(string PID, PCB p) {
	vaddr addr;
	/*if (p.parentPID != "")
		addr = fork_memory(p.parentPID, PID);
	else*/
		addr = Allocate_VM(PID, p.size);
	if (addr) {
		p.start = addr;
		recordTime();
		ofp << "进程创建" << endl;
		ofp << "进程名称：" << p.name << "  ";
		ofp << "进程标识：" << p.PID << "  ";
		if (p.type == processType::forSystem)
			ofp << "进程种类：系统进程" << "  ";
		else
			ofp << "进程种类：用户进程" << "  ";
		ofp << "运行时间：" << p.runtime << endl;
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

//进程删除函数
void destroyProcess(string PID) {
	int i = 0;
	bool tmp = getRunningProcess() == PID;								//判断需删除进程是否为正在执行进程
	PCB &p = getProcess(PID);										
	i = p.childProcess.size();
	while (i > 0) {														//父子进程级联删除
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
	if (tmp)															//若需删除进程为正在执行进程
		contextSwitch(p.type);											//上下文切换
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
	ofp << "进程名称：" << p.name << "  ";
	ofp << "进程标识：" << p.PID << "  ";
	if (p.type == processType::forSystem)
		ofp << "进程种类：系统进程" << endl;
	else
		ofp << "进程种类：用户进程" << endl;
	process.erase(PID);
	Free_VM(PID);														//释放进程所占空间
	if (tmp)															//若需删除进程为正在执行进程
		dispatcher();													//调度
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
			ofp << "进程阻塞解除" << endl;
			break;
		}
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		if (nextPList[i] != "") {
			PCB &nextProcess = (*(process.find(nextPList[i]))).second;
			ofp << "进程名称：" << nextProcess.name << "  ";
			ofp << "进程标识：" << nextProcess.PID << "  ";
			if (nextProcess.type == processType::forSystem)
				ofp << "进程种类：系统进程" << endl;
			else
				ofp << "进程种类：用户进程" << endl;
			outBL(nextProcess.PID, nextProcess.type);
			insertRL(nextProcess.PID, nextProcess.type);
		}
	}
	ofp << "进程删除" << endl;
	destroyProcess(PID);
}

//就绪队列插入函数
void insertRL(string PID, processType type) {
	switch (type) {														//根据进程种类，插入相应队列
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
	p.state = ready;													//更改进程属性
	p.list = readyL;
}

//就绪队列移出函数
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

//运行状态切换函数
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
	if (getRunningProcess() == "") {									//若无正在执行进程
		if (readyList[1].size() > 1)									//若系统进程等待队列非空
			intoRunning(processType::forSystem);						//取其首元素执行
		else if (readyList[0].size() > 1)								//否则，若用户进程等待队列非空
			intoRunning(processType::user);								//取其首元素执行
	}
}

//时间片轮转函数
void RR() {
	string s = getRunningProcess();
	if (s == "")														//若无正在执行进程
		return;
	PCB &p = (*(process.find(s))).second;	
	contextSwitch(p.type);												//否则，上下文切换
	insertRL(p.PID, p.type);											//该进程转为就绪状态
	dispatcher();														//调度
}

void annotation() {
	ofp.open("processM_log.txt", ios::out);
	ofp << "**************************************************************************************" << endl;
	ofp << ">>*                                                                  进程日志                                                                   *<<" << endl;
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