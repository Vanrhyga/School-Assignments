#include "bankPortal.h"


ifstream ifp;													//输入文件流
ofstream ofp;													//输出文件流
extern double systemTime;


void Account::readAnnotation() {
	int i;
	string buffer;
	ifp.open("File/Account.txt", ios::in);
	if (!ifp) {
		cout << "Account.txt无法打开！" << endl;
		return;
	}
	for (i = 1; i <= 14; i++)
		getline(ifp, buffer);
}

void Account::writeAnnotation() {
	string buffer;
	ifp.open("File/Annotation.txt", ios::in);
	if (!ifp) {
		cout << "Annotation.txt无法打开！" << endl;
		return;
	}
	ofp.open("File/Account.txt", ios::out);
	if (!ofp) {
		cout << "Account.txt无法打开！" << endl;
		return;
	}
	while (!ifp.eof()) {
		getline(ifp, buffer);
		ofp << buffer << endl;
	}
	ifp.close();
}

void Account::writeAccount() {	
	int i;
	ofp << cardID << endl;
	ofp << password << endl;
	ofp << state << endl;
	ofp << name << endl;
	ofp << ID << endl;
	ofp << phoneNumber << endl;
	ofp << bank << endl;
	ofp << balance << endl;
	ofp << transactionAmount << endl;
	if (!transactionAmount)
		ofp << setiosflags(ios::fixed) << setprecision(2) << transactionRecord[0];
	else
		for (i = 0; i < transactionAmount; i++) 
			ofp << setiosflags(ios::fixed) << setprecision(2) << transactionRecord[i] << "\t";
	ofp << endl;
	if (!transactionAmount)
		ofp << setiosflags(ios::fixed) << setprecision(2) << transactionTime[0];
	else
		for (i = 0; i < transactionAmount; i++) 
			ofp << setiosflags(ios::fixed) << setprecision(2) << transactionTime[i] << " \t";
	ofp << endl;
}

void Account::setCardID(char* cardID) {
	strcpy_s(this->cardID, cardID);
}

int Account::setPassword(string password) {
	int length = password.length();
	if (length != 6) {
		cout << "密码格式有误！其长度需为6位，且由数字组成：" << endl;
		return 1;
	}
	else {
		this->password = password;
		return 0;
	}
}

void Account::setName(string name) {
	this->name = name;
}

int Account::setID(string ID) {
	int length = ID.length();
	if (length != 18) {
		cout << "身份证号有误！请输入正确信息：" << endl;
		return 1;
	}
	else {
		this->ID = ID;
		return 0;
	}
}

int Account::setPhoneNumber(string phoneNumber) {
	int length = phoneNumber.length();
	if (length != 11) {
		cout << "手机号有误！请输入正确信息：" << endl;
		return 1;
	}
	else {
		this->phoneNumber = phoneNumber;
		return 0;
	}
}

void Account::setBank(string bank) {
	this->bank = bank;
}

void Account::setState() {
	state = 1;
}

void Account::generateCardID() {
	char cardID;
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < 19; i++) {
		cardID = rand() % 10 + 48;
		this->cardID[i] = cardID;
	}
	this->cardID[i] = '\0';
}

int Account::record(double sum) {
	if (balance + sum < 0)
		return 1;
	transactionRecord[transactionAmount++] = sum;
	balance += sum;
	return 0;
}

void Account::recordTime(double time) {
	transactionTime[transactionAmount-1] = time;
}

void Account::readAccount() {
	string buffer;
	int i;
	ifp >> cardID;
	getline(ifp, buffer);
	getline(ifp, password);
	ifp >> state;
	getline(ifp, buffer);
	getline(ifp, name);
	getline(ifp, ID);
	getline(ifp, phoneNumber);
	getline(ifp, bank);
	ifp >> balance;
	getline(ifp, buffer);
	ifp >> transactionAmount;
	getline(ifp, buffer);
	if (transactionAmount) 
		for (i = 0; i < transactionAmount; i++)
			ifp >> transactionRecord[i];
	else {
		transactionRecord[0] = 0;
		transactionRecord[1] = '\0';
	}
	getline(ifp, buffer);
	if (transactionAmount)
		for (i = 0; i < transactionAmount; i++)
			ifp >> transactionTime[i];
	else {
		transactionTime[0] = '0';
		transactionTime[1] = '\0';
	}
	getline(ifp, buffer);
	getline(ifp, buffer);
}

void Account::printAccountInfo() {
	int i, j;
	cout << "************************************" << bank << "卡************************************" << endl;
	cout << "卡号：";
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++)
			cout << cardID[i + j * 4];
		cout << " ";
	}
	cout << cardID[16] << cardID[17] << cardID[18] << endl;
	cout << "持卡人姓名：" << name << endl;
	cout << "持卡人身份证号：" << ID << endl;
	printf("余额：%.2f\n", balance);
	cout << "账户状态：";
	if (state)
		cout << "锁定" << endl;
	else
		cout << "正常" << endl;
	cout << "******************************************************************************************" << endl;
}

void Account::printTransactionRecord() {
	int i;
	cout << "******************************************交易记录*****************************************" << endl;
	for (i = 0; i < transactionAmount; i++) {
		cout << "交易时间：" << (int)transactionTime[i] << "月";
		printf("%d日\t", (int)((transactionTime[i] - (int)transactionTime[i]) * 100));
		cout << "金额：";
		if (transactionRecord[i] > 0)
			cout << "+";
		printf("%.2f\n", transactionRecord[i]);
	}
	cout << endl << "当前账户余额为：" << setiosflags(ios::fixed) << setprecision(2) << balance << endl;
	cout << "******************************************************************************************" << endl;
}

void Account::saveMoney() {
	double sum;
	cout << "******************************************************************************************" << endl;
	cout << "请输入存款金额：" << endl;
	cin >> sum;
	cout << "..." << endl;
	record(sum);
	recordTime(systemTime);
	Sleep(3000);
	cout << "存款成功！" << endl;
	cout << endl << "当前账户余额为：" << setiosflags(ios::fixed) << setprecision(2) << balance << endl;
	cout << "******************************************************************************************" << endl;
}

void Account::drawMoney() {
	double sum;
	cout << "******************************************************************************************" << endl;
	cout << "当前账户余额为：" << setiosflags(ios::fixed) << setprecision(2) << balance << endl << endl;
	cout << "请输入取款金额：" << endl;
	while (1) {
		cin >> sum;
		if (sum > 5000)
			cout << "超出每日最大额度——5000元！请重新输入金额：" << endl;
		else {
			cout << "..." << endl;
			sum = -sum;
			if (record(sum)) {
				cout << "账户余额不足！请重新输入金额：" << endl;
				continue;
			}
			recordTime(systemTime);
			Sleep(3000);
			cout << "取款成功！" << endl;
			cout << endl << "当前账户余额为：" << setiosflags(ios::fixed) << setprecision(2) << balance << endl;
			cout << "******************************************************************************************" << endl;
			break;
		}
	}
}

char* Account::getcardID() {
	return cardID;
}

string Account::getPassword() {
	return password;
}

int Account::getState() {
	return state;
}

Account::Account(){
	state = 0;
	balance = 0;
	transactionAmount = 0;
	transactionRecord[0] = 0;
	transactionTime[0] = 0;
}

Account::~Account(){
}
