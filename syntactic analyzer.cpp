#include<stdio.h>
#include<string>
#include<iostream>
using namespace std;


#define MAX 100																					//����ջ����									


char  stack[MAX];																				//����ջ
string table[5][8];																				//������


char isDigit(char);																				//�ж��Ƿ�Ϊ����
int map(char);																					//���ķ�����ӳ��Ϊ�±�
void initial();																					//��ʼ��������������
void printTable();																				//���������
int terminalSymbol(char);																		//�ж��Ƿ�Ϊ�ս��
int getLength();																				//���ջ����
void analyze(string);																			//Ԥ��������Ƴ���


int main() {
	string input;
	initial();																					//��ʼ��������
	cout << "***************************************************************************************************" << endl;
	cout << ">>                                            ������                                             <<" << endl;
	cout << "***************************************************************************************************" << endl;
	printTable();																				//���������
	cout << "***************************************************************************************************" << endl;
	cout << "��������������Ŵ���" << endl;														
	cin >> input;																				//����������ַ���
	input.append("$");																			
	cout << "***************************************************************************************************" << endl;
	cout << ">>                                           ��������                                            <<" << endl;
	cout << "***************************************************************************************************" << endl;
	analyze(input);
	system("pause");
	return 0;
}

char isDigit(char ch) {
	if (ch >= '0'&&ch <= '9')
		return 1;
	return ch;
}

int map(char ch) {
	switch (isDigit(ch)){
	case '+':
	case 'E':
		return 0;
	case '-':
	case 'A':
		return 1;
	case '*':
	case 'T':
		return 2;
	case '/':
	case 'B':
		return 3;
	case '(':
	case 'F':
		return 4;
	case ')':
		return 5;
	case 1:
		return 6;
	case '$':
		return 7;
	}
}

void initial() {
	table[map('E')][map('+')] = "error";
	table[map('E')][map('-')] = "error";
	table[map('E')][map('*')] = "error";
	table[map('E')][map('/')] = "error";
	table[map('E')][map('(')] = "E->TA";
	table[map('E')][map(')')] = "synch";
	table[map('E')][map('1')] = "E->TA";
	table[map('E')][map('$')] = "synch";
	table[map('A')][map('+')] = "A->+TA";
	table[map('A')][map('-')] = "A->-TA";
	table[map('A')][map('*')] = "error";
	table[map('A')][map('/')] = "error";
	table[map('A')][map('(')] = "error";
	table[map('A')][map(')')] = "A->epsilon";
	table[map('A')][map('1')] = "error";
	table[map('A')][map('$')] = "A->epsilon";
	table[map('T')][map('+')] = "synch";
	table[map('T')][map('-')] = "synch";
	table[map('T')][map('*')] = "error";
	table[map('T')][map('/')] = "error";
	table[map('T')][map('(')] = "T->FB";
	table[map('T')][map(')')] = "synch";
	table[map('T')][map('1')] = "T->FB";
	table[map('T')][map('$')] = "synch";
	table[map('B')][map('+')] = "B->epsilon";
	table[map('B')][map('-')] = "B->epsilon";
	table[map('B')][map('*')] = "B->*FB";
	table[map('B')][map('/')] = "B->/FB";
	table[map('B')][map('(')] = "error";
	table[map('B')][map(')')] = "B->epsilon";
	table[map('B')][map('1')] = "error";
	table[map('B')][map('$')] = "B->epsilon";
	table[map('F')][map('+')] = "synch";
	table[map('F')][map('-')] = "synch";
	table[map('F')][map('*')] = "synch";
	table[map('F')][map('/')] = "synch";
	table[map('F')][map('(')] = "F->(E)";
	table[map('F')][map(')')] = "synch";
	table[map('F')][map('1')] = "F->num";
	table[map('F')][map('$')] = "synch";
	stack[0] = '$';																				//'$'��ջ����ʶջ��
	stack[1] = 'E';																				//��ʼ������ջ
}

void printTable() {
	int i;
	printf("%15c%12c%12c%12c%12c%12c%12s%12c\n", '+', '-', '*', '/', '(', ')', "num", '$');
	cout << "   ------------------------------------------------------------------------------------------------" << endl;
	printf("%c %c", 'E','|');
	for (i = 0; i < 8; i++) 
		printf("  %10s", table[map('E')][i].c_str());
	printf("\n");
	printf("%c %c", 'A','|');
	for (i = 0; i < 8; i++)
		printf("  %10s", table[map('A')][i].c_str());
	printf("\n");
	printf("%c %c", 'T', '|');
	for (i = 0; i < 8; i++)
		printf("  %10s", table[map('T')][i].c_str());
	printf("\n");
	printf("%c %c", 'B','|');
	for (i = 0; i < 8; i++)
		printf("  %10s", table[map('B')][i].c_str());
	printf("\n");
	printf("%c %c", 'F','|');
	for (i = 0; i < 8; i++)
		printf("  %10s", table[map('F')][i].c_str());
	printf("\n"); 
}

int terminalSymbol(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || (isDigit(ch)==1))
		return 1;
	return 0;
}

int getLength() {
	int i;
	for (i = 0; i < MAX; i++)
		if (stack[i] == '\0')
			return i;
}

void analyze(string input) {
	char nonTerminal;
	int i,j;
	string::size_type index;
	printf("ջ\t\t\t\t\t      ����\t\t\t\t\t���\n");
	cout << "---------------------------------------------------------------------------------------------------" << endl;
	while (getLength()!=1) {
		if (terminalSymbol(stack[getLength() - 1])) {											//��ջ��Ϊ�ս��
			if (stack[getLength() - 1] == input[0]) {											//�����ս�����������ƥ��
				printf("%-15s%35s\n", stack, input.c_str());
				stack[getLength() - 1] = '\0';													//��ջ
				input=input.substr(1,input.length()-1);											//��ǰָ��ǰ��
			}
			else {																				//���򣬲�ƥ��
				printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(), "error");		//���������Ϣ
				return;																			//��������
			}
		}
		else {																					//����Ϊ���ս��
			if (!table[map(stack[getLength() - 1])][map(input[0])].compare("error")) {			//���������Ӧ����Ϊerror
				printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(), "error");		//���������Ϣ
				return;																			//��������
			}
			else {																				//����
				index = table[map(stack[getLength() - 1])][map(input[0])].find("epsilon");
				if (index == string::npos) {													//����Ӧ�����epsilon
					if (!table[map(stack[getLength() - 1])][map(input[0])].compare("synch")) {	//����Ӧ�����ͬ����Ϣ
						printf("%-15s%35s\n", stack, input.c_str());						
						stack[getLength() - 1] = '\0';											//��ջ
					}
					else {																		//���򣬸��ݷ�����ִ����Ӧ��������
						index = table[map(stack[getLength() - 1])][map(input[0])].find("num");
						printf("%-15s%35s   \t\t\t\t\t%-20s\n", stack, input.c_str(), table[map(stack[getLength() - 1])][map(input[0])].c_str());
						nonTerminal = stack[getLength() - 1];
						for (i = table[map(stack[getLength() - 1])][map(input[0])].length() - 1, stack[getLength() - 1] = '\0', j = getLength(); i > 2; i--,j++) {
							if (index != string::npos) {
								i -= 2;
								stack[j] = input[0];
								continue;
							}
							if (i == 1 || i == 2)
								continue;
							else
								stack[j] = table[map(nonTerminal)][map(input[0])][i];
						}
					}
				}
				else {																			//���򣬺���epsilon
					printf("%-15s%35s   \t\t\t\t\t%-20s\n", stack, input.c_str(), table[map(stack[getLength() - 1])][map(input[0])].c_str());
					stack[getLength() - 1] = '\0';												//��ջ
				}
			}
		}
	}
	printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(),"Accept");					//���������������ַ��������ķ�����
}