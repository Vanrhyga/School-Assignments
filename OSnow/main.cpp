#include "processM_head.h"
#include "mmu.h"
#include "vmm.h"
#include "fs_head.h"
/*
*                             _ooOoo_
*                            o8888888o
*                            88" . "88
*                            (| -_- |)
*                            O\  =  /O
*                         ____/`---'\____
*                       .'  \\|     |//  `.
*                      /  \\|||  :  |||//  \
*                     /  _||||| -:- |||||-  \
*                     |   | \\\  -  /// |   |
*                     | \_|  ''\---/''  |   |
*                     \  .-\__  `-`  ___/-. /
*                   ___`. .'  /--.--\  `. . __
*                ."" '<  `.___\_<|>_/___.'  >'"".
*               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
*               \  \ `-.   \_ __\ /__ _/   .-` /  /
*          ======`-.____`-.___\_____/___.-`____.-'======
*                             `=---='
*          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
*                     佛祖保佑        永无BUG
*/

extern map<string, PCB> process;
extern map<string, resource> allResource;
extern ofstream ofp;
int counter;
int timeSlot;
int allocMode;
int fileAmount = 0;
bool isRunning = TRUE;
std::mutex isOperating;

int main() {
	int i, j;
	string s;
	string command;
	counter = 1;
	Screen();
	initial();
	painting();
	annotation();
	srand((unsigned)time(NULL));
	for (i = 1; i <= MAX_RESOURCE_AMOUNT; i++) {
		j = rand() % MAX_SIZE + 1;
		allResource.insert(make_pair(toString(i), resource(toString(i), j, j)));
	}
	initList();
	CreateThread(NULL, 0, TIMER_SEC, NULL, 0, NULL);\
	CreateThread(NULL, 0, TIMER_NINE_SEC, NULL, 0, NULL);
	while (true) {
		cout << ">>";
		getline(cin, command);
		if (command == "open OSnow task manager") {
			while (TRUE) {
				pri();
			pri:
				Sleep(1000);
				system("cls");
				painting();
				cout << ">>********************************************************************************<<" << endl;
				cout << ">>*                             OSnow Task Manager                               *<<" << endl;
				cout << ">>********************************************************************************<<" << endl;
				if (_kbhit()) {
					pri();
					getline(cin, s);
					break;
				}
			}
			cout << ">>";
			getline(cin, command);
			if (command == "kill process") {
				pri();
				isRunning = FALSE;
				while (TRUE) {
					cout << "Please enter the PID:" << endl;
					cin >> s;
					if (process.find(s) != process.end())
						break;
					cout << "Error PID!" << endl;
				}
				recordTime();
				ofp << "用户操作 " << endl;
				killProcess(s);
				cout << "Process and its subprocesses have been killed!" << endl;
				isRunning = TRUE;
				goto pri;
			}
			else if (command == "exit to menu")
				continue;
			else {
				cout << "Error command!" << endl;
				goto pri;
			}
		}
		else if (command == "exit")
			break;
		else if (command == "check mmu") {
			show();
			while (TRUE) {
				string s;
				Sleep(1000);
				system("cls");
				painting();
				cout << ">>check mmu" << endl;
				show();
				if (_kbhit()) {
					getline(cin, s);
					break;
				}
			}
		}
		else if (command == "use file system")
			present();
		else
			cout << "Error command!" << endl;
	}
	cout << "Bye~" << endl << endl;
	ofp.close();
	system("pause");
	return 0;
}

//秒表
DWORD WINAPI TIMER_SEC(LPVOID lpparentet) {
	Sleep(8500);
	timeSlot = 0;
	while (TRUE) {
		Sleep(1000);
		while (!isRunning)
			Sleep(1000);
		timeSlot++;
		string s = getRunningProcess();
		if (s == "") {
			dispatcher();
			continue;
		}
		PCB &p = (*(process.find(s))).second;
		p.runtime--;
		if (!p.runtime) {
			recordTime();
			ofp << "非用户操作" << endl;
			killProcess(p.PID);
			timeSlot = 0;
		}
		if (timeSlot == 10) {
			int i;
			string nextPList[MAX_RESOURCE_AMOUNT];
			recordTime();
			ofp << "时间片轮转" << endl;
			for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
				nextPList[i] = "";
			p.releaseAllResource(nextPList);
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
			RR();
			timeSlot = 0;
		}
		if (timeSlot % 3 == 2) {
			int RID = rand() % MAX_RESOURCE_AMOUNT + 1;
			resource &r = getResource(toString(RID));
			int amount = rand() % r.amount + 1;
			recordTime();
			ofp << "进程资源申请" << endl;
			ofp << "进程名称：" << p.name << "  ";
			ofp << "进程标识：" << p.PID << "  ";
			if (p.type == processType::forSystem)
				ofp << "进程种类：系统进程" << "  ";
			else
				ofp << "进程种类：用户进程" << "  ";
			ofp << "资源种类：R" << RID << "  ";
			ofp << "资源数量：" << amount;
			int j = r.request(s, amount);
			if (j) {
				ofp << "（申请成功）" << endl;
				p.increaseResource(toString(RID), amount);
			}
			else {
				ofp << "（申请失败，阻塞）" << endl;
				int i;
				string nextPList[MAX_RESOURCE_AMOUNT];
				for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
					nextPList[i] = "";
				p.releaseAllResource(nextPList);
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
				contextSwitch(p.type);
				insertBL(s, p.type);
				dispatcher();
				timeSlot = 0;
			}
		}
		if (timeSlot % 3 == 1) {
			int oper = rand() % 2;
			size_vm tmp = rand() % p.size + 1;
			isOperating.lock();
			if (oper)
				Write_VM(p.PID, p.start + tmp, 128, p.buffer);
			else
				Read_VM(p.PID, p.start + tmp, 128, p.buffer);
			isOperating.unlock();
		}
		if (timeSlot % 5 == 2 && fileAmount <= 10) {
			isOperating.lock();
			int protect[3] = { 1, 1, 1 };
			int oper = rand() % 2;
			char dirName[10], fileName[10];
			if (oper) {
				int j = 0;
				for (int i = rand() % 8 + 1; i > 0; j++, i--) {
					char c = rand() % 26 + 97;
					dirName[j] = c;
				}
				dirName[j] = '\0';
				createDir(dirName);
			}
			int j = 0;
			for (int i = rand() % 8 + 1; i > 0; j++, i--) {
				char c = rand() % 26 + 97;
				fileName[j] = c;
			}
			fileName[j] = '\0';
			cd(dirName);
			createFile(fileName, protect);
			openFile(fileName);
			writeFile_auto(fileName, fileName);
			closeFile_auto(fileName);
			back();
			isOperating.unlock();
			recordTime();
			ofp << "写文件" << endl;
			ofp << "路径：..\\" << dirName << "\\" << fileName << "  " << "内容：" << fileName << endl;
			fileAmount++;
		}
	}
	return 0;
}

DWORD WINAPI TIMER_NINE_SEC(LPVOID lpparentet) {
	while (TRUE) {
		Sleep(9000);
		while (!isRunning)
			Sleep(9000);
		int i = rand() % 4;
		processType type;
		isOperating.lock();
		string s = getRunningProcess();
		if (s == "") {
			if (i == 0) {
				counter++;
				type = processType::forSystem;
				if (!insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type)))
					insertRL(toString(counter), type);
				else {
					recordTime();
					ofp << "空间不足，进程创建失败" << endl;
				}
			}
			else if (i == 1) {
				counter++;
				type = processType::user;
				if (!insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type)))
					insertRL(toString(counter), type);
				else {
					recordTime();
					ofp << "空间不足，进程创建失败" << endl;
				}
			}
			isOperating.unlock();
			continue;
		}
		PCB &p = (*(process.find(s))).second;
		if (i == 0) {
			counter++;
			type = processType::forSystem;
			if (!insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type)))
				insertRL(toString(counter), type);
			else {
				recordTime();
				ofp << "空间不足，进程创建失败" << endl;
			}
		}
		else if (i == 1) {
			counter++;
			type = processType::user;
			if (!insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type)))
				insertRL(toString(counter), type);
			else {
				recordTime();
				ofp << "空间不足，进程创建失败" << endl;
			}
		}
		else if (i == 2) {
			counter++;
			p.createChildP(toString(counter), "childOfp" + p.PID, p.type);
		}
		isOperating.unlock();
	}
	return 0;
}

void initial() {
	string s;
	while (TRUE) {
		cout << "Please select the alloc mode: " << endl;
		cout << "1.FirstFit\t2.BestFit\t3.WorstFit" << endl;
		cin >> allocMode;
		getline(cin, s);
		if (allocMode >= 1 && allocMode <= 3)
			break;
	}
	system("cls");
	Initblock();
	initialize();
	VMInit();
}

void Screen() {
	HWND hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN) - 1120;
	int y = GetSystemMetrics(SM_CYSCREEN) - 300;
	char setting[30];
	sprintf_s(setting, "mode con:cols=%d lines=%d", x, y);
	system(setting);
	MoveWindow(hwnd, 300, 0, x + 300, y + 300, 1);
}