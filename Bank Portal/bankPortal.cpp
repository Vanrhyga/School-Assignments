#include "bankPortal.h"


double systemTime;									//ϵͳʱ��
int pastDay = 0;				
extern ifstream ifp;								//�����ļ���
extern ofstream ofp;								//����ļ���

void readSystemTime();
void incSystemTime();
void writeSystemTime();

int main() {
	char cardID[21];
	int accountAmount;
	int i;
	int choice, bankChoice;
	int times1, times2 = 1;
	double amount = 0;
	clock_t startTime, curTime;
	double pastTime = 0;
	string name;
	string ID;
	string phoneNumber;
	string password;
	string confirmPassword;
	Bank banks[7];
	Account newAccount;
	Account accounts[MAX_ACCOUNT_AMOUNT];
	readSystemTime();
	Account::readAnnotation();
	for (accountAmount = 0; !ifp.eof(); accountAmount++)
		accounts[accountAmount].readAccount();
	ifp.close();
	cout << "******************************************************************************************" << endl;
	cout << ">>*                               ��ӭ��½������������ϵͳ                             *<<" << endl;
	cout << "******************************************************************************************" << endl;
	while(1){
		cout << "��ϵͳ�ṩ�ķ����������£�" << endl;
		cout << "1.ע�����п�   \t\t" << "2.���" << endl;
		cout << "3.ȡ��        \t\t" << "4.�޸�����" << endl;
		cout << "5.�޸�Ԥ���ֻ� \t\t" << "6.��ѯ���׼�¼" << endl;
		cout << "7.��ʧ        \t\t" << "8.�˳�ϵͳ" << endl;
		cout << endl;
		startTime = clock();
		cout << "��ѡ������Ҫ�ķ���" << endl;
		start:
		cin >> choice;
		if (choice == 1) {
			cout << "******************************************************************************************" << endl;
			ifp.open("File/Bank.txt", ios::in);
			if (!ifp) {
				cout << "Bank.txt�޷��򿪣�" << endl;
				return 0;
			}
			for (i = 0; i < 7; i++)
				banks[i].read();
			ifp.close();
			cout << "��ע����������£�" << endl;
			for (i = 0; i < 7; i++)
				cout << i + 1 << "." << banks[i].getName() << endl;
			cout << endl << "��ѡ��" << endl;
			while (1) {
				cin >> bankChoice;
				if (bankChoice >= 1 && bankChoice <= 7) {
					newAccount.setBank(banks[bankChoice - 1].getName());
					break;
				}
				else
					cout << endl << "���������������������ѡ��" << endl;
			}
			cout << endl << "����������������" << endl;
			cin >> name;
			newAccount.setName(name);
			cout << "�������������֤�ţ�" << endl;
			while (1) {
				cin >> ID;
				if (!newAccount.setID(ID))
					break;
			}
			cout << "����������Ԥ���ֻ���" << endl;
			while (1) {
				cin >> phoneNumber;
				if (!newAccount.setPhoneNumber(phoneNumber))
					break;
			}
			cout << "�������������룺" << endl;
			while (1) {
				while (1) {
					cin >> password;
					if (!newAccount.setPassword(password))
						break;
				}
				cout << "���ٴ�ȷ���������룺" << endl;
				cin >> confirmPassword;
				if (confirmPassword == password)
					break;
				else
					cout << "�������벻��ͬ�������������������룺" << endl;
			}
			newAccount.generateCardID();
			cout << endl << ">>*                                   ���п�ע��ɹ�                                   *<<" << endl << endl;
			newAccount.printAccountInfo();
			accounts[accountAmount++] = newAccount;
		}
		else if (choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7) {
			if (times2 > 1)
				goto next;
			cout << "******************************************************************************************" << endl;
			cout << "���������Ŀ��ţ�" << endl;
			while (1) {
				while (1) {
					cin >> cardID;
					if (strlen(cardID) == 19)
						break;
					else
						cout << "���������������������룺" << endl;
				}
				for (i = 0; i < accountAmount; i++)
					if (!strcmp(accounts[i].getcardID(), cardID))
						break;
				if (i == accountAmount) {
					cout << "�����ڸ��˻������������뿨�ţ�" << endl;
					continue;
				}
				password = accounts[i].getPassword();
				break;
			}
			cout << "�������������룺" << endl;
			for (times1 = 1; times1 <= 3; times1++) {
				cin >> confirmPassword;
				if (confirmPassword == password)
					break;
				else
					cout << "�����������������룺" << endl;
			}
			if (times1 > 3) {
				accounts[i].setState();															
				cout << endl << "�����˻��ѱ����ᣬ��ƾ��Ч֤������̨��������" << endl;
				cout << "******************************************************************************************" << endl;
				continue;
			}
			next:
			if (accounts[i].getState()) {
				cout << endl << "���˻����ڶ���״̬���޷����в�������ƾ��Ч֤������̨��������" << endl;
				cout << "******************************************************************************************" << endl;
				continue;
			}
			else {
				if (choice == 2) {
					curTime = clock();
					pastTime += (curTime - startTime) / (3 * CLOCKS_PER_SEC);
					pastDay = (int)pastTime;
					incSystemTime();
					pastTime -= pastDay;
					accounts[i].saveMoney();
				}
				else if (choice == 3) {
					curTime = clock();
					pastTime += (curTime - startTime) / (3 * CLOCKS_PER_SEC);
					pastDay = (int)pastTime;
					incSystemTime();
					pastTime -= pastDay;
					accounts[i].drawMoney();
				}
				else if (choice == 4 || choice == 5 || choice == 7) {
					cout << "******************************************************************************************" << endl;
					if (choice == 7)
						cout << "��ʹ��Ԥ���ֻ������� ��gs�� ��15601282822" << endl;
					else
						cout << "��ʹ��Ԥ���ֻ������� ��xg�� ��15601282822" << endl;
					cout << "..." << endl;
					Sleep(8000);
					cout << "���յ����ţ�" << endl;
					if (choice == 4) {
						cout << endl << "�������µ����룺" << endl;
						while (1) {
							while (1) {
								cin >> password;
								if (!accounts[i].setPassword(password))
									break;
							}
							cout << "���ٴ�ȷ���������룺" << endl;
							cin >> confirmPassword;
							if (confirmPassword == password)
								break;
							else
								cout << "�������벻��ͬ�������������µ����룺" << endl;
						}
						cout << endl << "�����޸ĳɹ���" << endl;
						cout << "******************************************************************************************" << endl;
					}
					else if (choice == 5) {
						cout << endl << "�������µ��ֻ��ţ�" << endl;
						while (1) {
							cin >> phoneNumber;
							if (!accounts[i].setPhoneNumber(phoneNumber))
								break;
						}
						cout << endl << "Ԥ���ֻ����޸ĳɹ���" << endl;
						cout << "******************************************************************************************" << endl;
					}
					else {
						accounts[i].setState();
						cout << endl << "��ʧ�ɹ��������ң���ƾ��Ч֤������̨����" << endl;
						cout << "******************************************************************************************" << endl;
					}
				}
				else if (choice == 6)
					accounts[i].printTransactionRecord();
			}
			times2++;
		}
		else if (choice == 8) {
			cout << "******************************************************************************************" << endl;
			cout << ">>*                                    ��л����ʹ��                                    <<*" << endl;
			cout << "******************************************************************************************" << endl;
			break;
		}
		else {
			cout << endl << "��������������������������룺" << endl;
			goto start;
		}
	}
	Account::writeAnnotation();
	for (i = 0; i < accountAmount; i++)
		accounts[i].writeAccount();
	ofp.close();
	curTime = clock();
	pastTime += (curTime - startTime) / (3 * CLOCKS_PER_SEC);
	pastDay = (int)pastTime;
	incSystemTime();
	writeSystemTime();
	cout << endl;
	system("pause");
	return 0;
}

void readSystemTime() {
	ifp.open("File/System Time.txt", ios::in);
	ifp >> systemTime;
	ifp.close();
}

void incSystemTime() {
	int month = (int)systemTime;
	double day = (systemTime - (int)systemTime) * 100;
	if (month == 2 && day + pastDay > 28)
		systemTime = 3 + (day + pastDay - 28) / 100;
	else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day + pastDay > 31)) {
		if (month == 12)
			systemTime = 1 + (day + pastDay - 31) / 100;
		else
			systemTime = ++month + (day + pastDay - 31) / 100;
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day + pastDay > 30))
		systemTime = ++month + (day + pastDay - 30) / 100;
	else
		systemTime += pastDay / 100.0;
}

void writeSystemTime() {
	ofp.open("File/System Time.txt", ios::out);
	ofp << setiosflags(ios::fixed) << setprecision(2) << systemTime;
	ofp.close();
}