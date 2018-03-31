#include "processM_head.h"
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
*                     ·ð×æ±£ÓÓ        ÓÀÎÞBUG
*/

extern map<string, PCB> process;
extern map<string, resource> allResource;
int counter;
int timeSlot;

int main() {
	int i, j;
	string s;
	string command;
	counter = 1;
	painting();
	srand((unsigned)time(NULL));
	for (i = 1; i <= MAX_RESOURCE_AMOUNT; i++) {
		j = rand() % MAX_SIZE + 1;
		allResource.insert(make_pair(toString(i), resource(toString(i), j, j)));
	}
	initList();
	CreateThread(NULL, 0, TIMER_SEC, NULL, 0, NULL);
	CreateThread(NULL, 0, TIMER_NINE_SEC, NULL, 0, NULL);
	while (true) {
		s = getRunningProcess();
		if (s == "")
			break;
		PCB &p = (*(process.find(s))).second;
		cout << ">>";
		getline(cin, command);
		if (command == "kill process") {
			string name;
			cout << "Please enter the process name:" << endl;
			cin >> name;
			getline(cin, s);
			s = nametoPID(name);
			killProcess(s);
		}
		else if (command == "open OSnow task manager") {
			pri();
		}
		else if (command == "exit")
			break;
		else 
			cout << "Error command!" << endl;
	}
	cout << "Bye~" << endl << endl;
	system("pause");
	return 0;
}

DWORD WINAPI TIMER_SEC(LPVOID lpparentet) {
	timeSlot = 0;
	while (TRUE) {
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
			killProcess(p.PID);
			timeSlot = 0;
		}
		if (timeSlot == 10) {
			int i;
			string nextPList[MAX_RESOURCE_AMOUNT];
			for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
				nextPList[i] = "";
			p.releaseAllResource(nextPList);
			for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
				if (nextPList[i] != "") {
					PCB &nextProcess = (*(process.find(nextPList[i]))).second;
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
			int j = r.request(s, amount);
			if (j)
				p.increaseResource(toString(RID), amount);
			else {
				int i;
				string nextPList[MAX_RESOURCE_AMOUNT];
				for (i = 0; i < MAX_RESOURCE_AMOUNT; i++)
					nextPList[i] = "";
				p.releaseAllResource(nextPList);
				for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
					if (nextPList[i] != "") {
						PCB &nextProcess = (*(process.find(nextPList[i]))).second;
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
	}
	return 0;
}

DWORD WINAPI TIMER_NINE_SEC(LPVOID lpparentet) {
	while (TRUE) {
		Sleep(9000);
		int i = rand() % 4;
		processType type;
		string s = getRunningProcess();
		if (s == "") {
			if (i == 0) {
				counter++;
				type = processType::forSystem;
				insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type));
				insertRL(toString(counter), type);
			}
			else if (i == 1) {
				counter++;
				type = processType::user;
				insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type));
				insertRL(toString(counter), type);
			}
			continue;
		}
		PCB &p = (*(process.find(s))).second;
		if (i == 0) {
			counter++;
			type = processType::forSystem;
			insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type));
			insertRL(toString(counter), type);
		}
		else if (i == 1) {
			counter++;
			type = processType::user;
			insertProcess(toString(counter), PCB(toString(counter), "p" + toString(counter), type));
			insertRL(toString(counter), type);
		}
		else if (i == 2) {
			counter++;
			p.createChildP(toString(counter), "childOfp" + p.PID, p.type);
		}
	}
	return 0;
}