#pragma once
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<Windows.h>
#include<time.h>
using namespace std;


#define MAX_ACCOUNT_AMOUNT 100 				//����˻�����


class Account{								//�˻�
private:
	char cardID[20];						//���ţ���Ϊ19λ
	string password;						//���루��Ϊ6λ
	int state;								//���п�״̬
	string name;							//�ֿ�������
	string ID;								//�ֿ������֤�ţ���Ϊ18λ
	string phoneNumber;						//Ԥ���ֻ��ţ���Ϊ11λ
	string bank;							//��������
	double balance;							//�������
	int transactionAmount;					//���״���
	double transactionRecord[30];			//���׽��
	double transactionTime[30];				//����ʱ��
public:
	static void readAnnotation();			//����ע����Ϣ
	static void writeAnnotation();			//���ļ�д��ע����Ϣ
	void writeAccount();					//���ļ�д���˻���Ϣ
	void setCardID(char*);					//���ÿ���
	int setPassword(string);				//��������
	void setName(string);					//���óֿ�������
	int setID(string);						//���óֿ������֤��
	int setPhoneNumber(string);				//����Ԥ���ֻ���
	void setBank(string);					//������������
	void setState();						//�����˻�
	void generateCardID();					//������ɿ���
	int record(double);						//��¼���׽��
	void recordTime(double);				//��¼����ʱ��
	void readAccount();						//���ļ���ȡ�˻���Ϣ
	void printAccountInfo();				//��ӡ�˻���Ϣ
	void printTransactionRecord();			//��ӡ���׼�¼
	void saveMoney();						//���
	void drawMoney();						//ȡ��
	char* getcardID();						//��ÿ���
	string getPassword();					//�������
	int getState();							//����˻�״̬
	Account();
	~Account();
};

class Bank{									//����
private:
	string name;							//����
public:
	void read();							//���ļ���ȡ��Ϣ
	string getName();						//�����������
	Bank();
	~Bank();
};