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

int main() {
	int i, j;
	string s;
	string command;
	counter = 1;
	srand((unsigned)time(NULL));
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		j = rand() % (MAX_SIZE + 1);
		allResource.insert(make_pair(toString(i), resource(toString(i), j, j)));
	}
	initList();
	CreateThread(NULL, 0, TIMER_SEC, NULL, 0, NULL);
	CreateThread(NULL, 0, TIMER_FIVE_SEC, NULL, 0, NULL);
	CreateThread(NULL, 0, TIMER_TEN_SEC, NULL, 0, NULL);
	while (true) {
		s = getRunningProcess();
		if (s == "")
			break;
		PCB &p = (*(process.find(s))).second;
		cout << ">>";
		getline(cin, command);
		if (command == "create child process") {
			string name;
			cout << "Please enter the name of the child process:" << endl;
			cin >> name;
			getline(cin, s);
			counter++;
			p.createChildP(toString(counter), name, p.type);
		}
		else if (command == "timeout")
			RR();
		else if (command == "request resources") {
			string name;
			int amount;
			cout << "Please enter the resource name:" << endl;
			cin >> name;
			cout << "Please input the amount of resources requested:" << endl;
			cin >> amount;
			getline(cin, s);
			resource &r = getResource(name);
			i = r.request(p.PID, amount);
			if (!i) {
				insertBL(p.PID, p.type);
				dispatcher();
			}
			else if (i == 1)
				p.increaseResource(name, amount);
			else 
				cout << "Error! Requested resource exceeds upper limit!" << endl;
		}
		else if (command == "release resources") {
			string name;
			cout << "Please enter the resource name:" << endl;
			cin >> name;
			getline(cin, s);
			resource &r = getResource(name);
			s = r.release(p.countResource(name));
			if (s != "") {
				PCB &nextProcess = (*(process.find(s))).second;
				outBL(nextProcess.PID, nextProcess.type);
				insertRL(nextProcess.PID, nextProcess.type);
			}
		}
		else if (command == "kill process") {
			string name;
			cout << "Please enter the process name:" << endl;
			cin >> name;
			getline(cin, s);
			s = nametoPID(name);
			killProcess(s);
		}
		else if (command == "list processes") {
			auto iter = process.begin();
			while (iter != process.end()) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.state == ready)
					cout << "ready::";
				else if (iter->second.state == running)
					cout << "running::";
				else
					cout << "blocked::";
				if (iter->second.type == user)
					cout << "user" << endl;
				else
					cout << "system" << endl;
				iter++;
			}
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
	while (TRUE) {
		Sleep(1000);
		string s = getRunningProcess();
		if (s == "") {
			dispatcher();
			continue;
		}
		PCB &p = (*(process.find(s))).second;
		p.runtime--;
//ÏÔÊ¾
		cout << p.runtime << endl;
		cout << p.name << endl;
		cout << p.type << endl;
		auto iter = process.begin();
		while (iter != process.end()) {
			cout << iter->second.name << "::" << iter->first << "::";
			if (iter->second.state == ready)
				cout << "ready::";
			else if (iter->second.state == running)
				cout << "running::";
			else
				cout << "blocked::";
			if (iter->second.type == user)
				cout << "user" << endl;
			else
				cout << "system" << endl;
			iter++;
		}

		if (!p.runtime)
			killProcess(p.PID);
	}
	return 0;
}

DWORD WINAPI TIMER_FIVE_SEC(LPVOID lpparentet) {
	while (TRUE) {
		Sleep(5000);
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

DWORD WINAPI TIMER_TEN_SEC(LPVOID lpparentet) {
	return 0;
}