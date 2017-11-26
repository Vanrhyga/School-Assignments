#include<stdio.h>
#include<string>
#include<iostream>
using namespace std;


#define MAX 100																					//符号栈容量									


char  stack[MAX];																				//符号栈
string table[5][8];																				//分析表


char isDigit(char);																				//判断是否为数字
int map(char);																					//将文法符号映射为下标
void initial();																					//初始化，构建分析表
void printTable();																				//输出分析表
int terminalSymbol(char);																		//判断是否为终结符
int getLength();																				//获得栈长度
void analyze(string);																			//预测分析控制程序


int main() {
	string input;
	initial();																					//初始化分析表
	cout << "***************************************************************************************************" << endl;
	cout << ">>                                            分析表                                             <<" << endl;
	cout << "***************************************************************************************************" << endl;
	printTable();																				//输出分析表
	cout << "***************************************************************************************************" << endl;
	cout << "请输入待分析符号串：" << endl;														
	cin >> input;																				//读入待分析字符串
	input.append("$");																			
	cout << "***************************************************************************************************" << endl;
	cout << ">>                                           分析过程                                            <<" << endl;
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
	stack[0] = '$';																				//'$'入栈，标识栈底
	stack[1] = 'E';																				//开始符号入栈
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
	printf("栈\t\t\t\t\t      输入\t\t\t\t\t输出\n");
	cout << "---------------------------------------------------------------------------------------------------" << endl;
	while (getLength()!=1) {
		if (terminalSymbol(stack[getLength() - 1])) {											//若栈顶为终结符
			if (stack[getLength() - 1] == input[0]) {											//若该终结符与输入符号匹配
				printf("%-15s%35s\n", stack, input.c_str());
				stack[getLength() - 1] = '\0';													//弹栈
				input=input.substr(1,input.length()-1);											//向前指针前移
			}
			else {																				//否则，不匹配
				printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(), "error");		//输出错误信息
				return;																			//分析结束
			}
		}
		else {																					//否则，为非终结符
			if (!table[map(stack[getLength() - 1])][map(input[0])].compare("error")) {			//若分析表对应表项为error
				printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(), "error");		//输出错误信息
				return;																			//分析结束
			}
			else {																				//否则
				index = table[map(stack[getLength() - 1])][map(input[0])].find("epsilon");
				if (index == string::npos) {													//若对应表项不含epsilon
					if (!table[map(stack[getLength() - 1])][map(input[0])].compare("synch")) {	//若对应表项含有同步信息
						printf("%-15s%35s\n", stack, input.c_str());						
						stack[getLength() - 1] = '\0';											//弹栈
					}
					else {																		//否则，根据分析表执行相应分析动作
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
				else {																			//否则，含有epsilon
					printf("%-15s%35s   \t\t\t\t\t%-20s\n", stack, input.c_str(), table[map(stack[getLength() - 1])][map(input[0])].c_str());
					stack[getLength() - 1] = '\0';												//弹栈
				}
			}
		}
	}
	printf("%-15s%35s   \t\t\t\t\t%-20s\n\n", stack, input.c_str(),"Accept");					//分析结束，输入字符串符合文法规则
}