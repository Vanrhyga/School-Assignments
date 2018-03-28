#include "processM_head.h"

extern map<string, PCB> process;
extern map<string, resource> allResource;

int main() {
	int i, j;
	int counter = 1;
	string s;
	string command;
	srand((unsigned)time(NULL));
	for (i = 0; i < MAX_RESOURCE_AMOUNT; i++) {
		j = rand() % (MAX_SIZE + 1);
		allResource.insert(make_pair(toString(i), resource(toString(i), j, j)));
	}
	initList();
	j = rand() % (MAX_PROCESS_AMOUNT + 1);
	for (i = 0; i < j; i++, counter++) {
		processType type;
		if (rand() % 2)
			type = processType::forSystem;
		else
			type = processType::user;
		PCB p1(toString(counter), "p"+toString(counter), type);
		insertProcess(p1.PID, p1);
		insertRL(p1.PID, p1.type);
	}
	dispatcher();
	while (true) {
		s = getRunningProcess();
		PCB &p = (*(process.find(s))).second;
		cout << ">>";
		getline(cin, command);
		if (command == "create child process") {
			string name;
			cout << "Please enter the name of the child process:" << endl;
			cin >> name;
			getline(cin, s);
			p.createChildP(toString(counter), name, p.type);
			counter++;
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
			PCB &tmp = getProcess(s);
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