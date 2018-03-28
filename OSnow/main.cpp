#include "processM_head.h"

extern map<string, PCB> process;
extern map<string, resource> allResource;

int main() {
	int i, j;
	int counter = 1;
	string s;
	string command;
	srand((unsigned)time(NULL));
	for (i = 0; i < MAX_SIZE; i++) {
		j = rand() % MAX_SIZE;
		allResource.insert(make_pair(toString(i), resource(toString(i), j, j)));
	}
	initList();
	PCB p1(toString(counter), "p1", processType::user);
	insertProcess(p1.PID, p1);
	insertRL(p1.PID, p1.type);
	dispatcher();
	cout << "p1 is running!" << endl;
	while (true) {
		s = getRunningProcess();
		PCB &p = (*(process.find(s))).second;
		cin >> command;
		if (command == "create child process") {
			string name;
			cout << "Please enter the name of the child process:" << endl;
			cin >> name;
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
			int amount;
			cout << "Please enter the resource name:" << endl;
			cin >> name;
			cout << "Please input the amount of resources released:" << endl;
			cin >> amount;
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
			s = nametoPID(name);
			PCB &tmp = getProcess(s);
			string nextPList[MAX_SIZE];
			for (i = 0; i < MAX_SIZE; i++)
				nextPList[i] = "";
			tmp.releaseAllResource(nextPList);
			for (i = 0; i < MAX_SIZE; i++) {
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
			if (getRunningProcess() == tmp.PID)
				dispatcher();
			destroyProcess(tmp.PID);
		}
		else if (command == "list processes") {
			auto iter = process.begin();
			while (iter != process.end()) {
				cout << iter->second.name << "::" << iter->first << "::";
				if (iter->second.state == ready)
					cout << "ready" << endl;
				else if (iter->second.state == running)
					cout << "running" << endl;
				else
					cout << "blocked" << endl;
				iter++;
			}
		}
		else if (command == "exit")
			break;
	}
	cout << endl << "END" << endl;
	return 0;
}