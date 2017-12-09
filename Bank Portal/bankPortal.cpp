#include "bankPortal.h"


double systemTime;									//系统时间
int pastDay = 0;				
extern ifstream ifp;								//输入文件流
extern ofstream ofp;								//输出文件流

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
	cout << ">>*                               欢迎登陆银行自助服务系统                             *<<" << endl;
	cout << "******************************************************************************************" << endl;
	while(1){
		cout << "本系统提供的服务类型如下：" << endl;
		cout << "1.注册银行卡   \t\t" << "2.存款" << endl;
		cout << "3.取款        \t\t" << "4.修改密码" << endl;
		cout << "5.修改预留手机 \t\t" << "6.查询交易记录" << endl;
		cout << "7.挂失        \t\t" << "8.退出系统" << endl;
		cout << endl;
		startTime = clock();
		cout << "请选择您需要的服务：" << endl;
		start:
		cin >> choice;
		if (choice == 1) {
			cout << "******************************************************************************************" << endl;
			ifp.open("File/Bank.txt", ios::in);
			if (!ifp) {
				cout << "Bank.txt无法打开！" << endl;
				return 0;
			}
			for (i = 0; i < 7; i++)
				banks[i].read();
			ifp.close();
			cout << "可注册的银行如下：" << endl;
			for (i = 0; i < 7; i++)
				cout << i + 1 << "." << banks[i].getName() << endl;
			cout << endl << "请选择：" << endl;
			while (1) {
				cin >> bankChoice;
				if (bankChoice >= 1 && bankChoice <= 7) {
					newAccount.setBank(banks[bankChoice - 1].getName());
					break;
				}
				else
					cout << endl << "银行序号输入有误！请重新选择：" << endl;
			}
			cout << endl << "请输入您的姓名：" << endl;
			cin >> name;
			newAccount.setName(name);
			cout << "请输入您的身份证号：" << endl;
			while (1) {
				cin >> ID;
				if (!newAccount.setID(ID))
					break;
			}
			cout << "请输入您的预留手机：" << endl;
			while (1) {
				cin >> phoneNumber;
				if (!newAccount.setPhoneNumber(phoneNumber))
					break;
			}
			cout << "请输入您的密码：" << endl;
			while (1) {
				while (1) {
					cin >> password;
					if (!newAccount.setPassword(password))
						break;
				}
				cout << "请再次确认您的密码：" << endl;
				cin >> confirmPassword;
				if (confirmPassword == password)
					break;
				else
					cout << "两次密码不相同！请重新输入您的密码：" << endl;
			}
			newAccount.generateCardID();
			cout << endl << ">>*                                   银行卡注册成功                                   *<<" << endl << endl;
			newAccount.printAccountInfo();
			accounts[accountAmount++] = newAccount;
		}
		else if (choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7) {
			if (times2 > 1)
				goto next;
			cout << "******************************************************************************************" << endl;
			cout << "请输入您的卡号：" << endl;
			while (1) {
				while (1) {
					cin >> cardID;
					if (strlen(cardID) == 19)
						break;
					else
						cout << "卡号输入有误！请重新输入：" << endl;
				}
				for (i = 0; i < accountAmount; i++)
					if (!strcmp(accounts[i].getcardID(), cardID))
						break;
				if (i == accountAmount) {
					cout << "不存在该账户！请重新输入卡号：" << endl;
					continue;
				}
				password = accounts[i].getPassword();
				break;
			}
			cout << "请输入您的密码：" << endl;
			for (times1 = 1; times1 <= 3; times1++) {
				cin >> confirmPassword;
				if (confirmPassword == password)
					break;
				else
					cout << "密码有误！请重新输入：" << endl;
			}
			if (times1 > 3) {
				accounts[i].setState();															
				cout << endl << "您的账户已被冻结，请凭有效证件至柜台处解锁！" << endl;
				cout << "******************************************************************************************" << endl;
				continue;
			}
			next:
			if (accounts[i].getState()) {
				cout << endl << "该账户处于冻结状态，无法进行操作，请凭有效证件至柜台处解锁！" << endl;
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
						cout << "请使用预留手机，发送 “gs” 至15601282822" << endl;
					else
						cout << "请使用预留手机，发送 “xg” 至15601282822" << endl;
					cout << "..." << endl;
					Sleep(8000);
					cout << "已收到短信！" << endl;
					if (choice == 4) {
						cout << endl << "请输入新的密码：" << endl;
						while (1) {
							while (1) {
								cin >> password;
								if (!accounts[i].setPassword(password))
									break;
							}
							cout << "请再次确认您的密码：" << endl;
							cin >> confirmPassword;
							if (confirmPassword == password)
								break;
							else
								cout << "两次密码不相同！请重新输入新的密码：" << endl;
						}
						cout << endl << "密码修改成功！" << endl;
						cout << "******************************************************************************************" << endl;
					}
					else if (choice == 5) {
						cout << endl << "请输入新的手机号：" << endl;
						while (1) {
							cin >> phoneNumber;
							if (!accounts[i].setPhoneNumber(phoneNumber))
								break;
						}
						cout << endl << "预留手机号修改成功！" << endl;
						cout << "******************************************************************************************" << endl;
					}
					else {
						accounts[i].setState();
						cout << endl << "挂失成功！如需解挂，请凭有效证件至柜台处。" << endl;
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
			cout << ">>*                                    感谢您的使用                                    <<*" << endl;
			cout << "******************************************************************************************" << endl;
			break;
		}
		else {
			cout << endl << "服务序号输入有误！请您重新输入：" << endl;
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