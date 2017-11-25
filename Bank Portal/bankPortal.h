#pragma once
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<Windows.h>
#include<time.h>
using namespace std;


#define MAX_ACCOUNT_AMOUNT 100 				//最大账户数量


class Account{								//账户
private:
	char cardID[20];						//卡号（必为19位
	string password;						//密码（必为6位
	int state;								//银行卡状态
	string name;							//持卡人姓名
	string ID;								//持卡人身份证号（必为18位
	string phoneNumber;						//预留手机号（必为11位
	string bank;							//所属银行
	double balance;							//卡内余额
	int transactionAmount;					//交易次数
	double transactionRecord[30];			//交易金额
	double transactionTime[30];				//交易时间
public:
	static void readAnnotation();			//跳过注释信息
	static void writeAnnotation();			//向文件写入注释信息
	void writeAccount();					//向文件写入账户信息
	void setCardID(char*);					//设置卡号
	int setPassword(string);				//设置密码
	void setName(string);					//设置持卡人姓名
	int setID(string);						//设置持卡人身份证号
	int setPhoneNumber(string);				//设置预留手机号
	void setBank(string);					//设置所属银行
	void setState();						//冻结账户
	void generateCardID();					//随机生成卡号
	int record(double);						//记录交易金额
	void recordTime(double);				//记录交易时间
	void readAccount();						//从文件读取账户信息
	void printAccountInfo();				//打印账户信息
	void printTransactionRecord();			//打印交易记录
	void saveMoney();						//存款
	void drawMoney();						//取款
	char* getcardID();						//获得卡号
	string getPassword();					//获得密码
	int getState();							//获得账户状态
	Account();
	~Account();
};

class Bank{									//银行
private:
	string name;							//名称
public:
	void read();							//从文件读取信息
	string getName();						//获得银行名称
	Bank();
	~Bank();
};