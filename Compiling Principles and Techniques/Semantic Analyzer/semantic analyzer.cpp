#include<stdio.h>
#include<string>
#include<iostream>
#include<vector>
#include<sstream>
using namespace std;


string table[18][12];																	//������action&goto��
vector<char> stack;																		//����ջ
vector<int> state;																		//״̬ջ
vector<double> value;																	//����ջ
vector<string> type;


char isDigit(char);																		//�ж��Ƿ�Ϊ����
int map(char);																			//���ķ�����ӳ��Ϊ�±�
void initial();																			//��ʼ��������������
void printTable();																		//���������
void analyze(string);																	//����������������


int main() {
	string input;
	initial();																			//��ʼ��������																		
	cout << "***************************************************************************************************************************************************************************************" << endl;
	cout << ">>                                                                                      ������                                                                                      <<" << endl;
	cout << "***************************************************************************************************************************************************************************************" << endl;
	printTable();																		//���������
	cout << "***************************************************************************************************************************************************************************************" << endl;
	cout << "��������������Ŵ���" << endl;
	cin >> input;																		//����������ַ���
	input.append("$");
	cout << "***************************************************************************************************************************************************************************************" << endl;
	cout << ">>                                                                                     ��������                                                                                     <<" << endl;
	cout << "***************************************************************************************************************************************************************************************" << endl;
	analyze(input);																		//�������������
	system("pause");
	return 0;
}

char isDigit(char ch) {
	if (ch >= '0'&&ch <= '9')
		return 1;
	return ch;
}

int map(char ch) {
	switch (isDigit(ch)) {
	case '+':
		return 0;
	case '-':
		return 1;
	case '*':
		return 2;
	case '/':
		return 3;
	case 1:
		return 4;
	case '.':
		return 5;
	case '(':
		return 6;
	case ')':
		return 7;
	case '$':
		return 8;
	case 'E':
		return 9;
	case 'T':
		return 10;
	case 'F':
		return 11;
	}
}

void initial() {
	table[0][map('+')] = "error";
	table[0][map('-')] = "error";
	table[0][map('*')] = "error";
	table[0][map('/')] = "error";
	table[0][map('1')] = "S4";
	table[0][map('.')] = "error";
	table[0][map('(')] = "S5";
	table[0][map(')')] = "error";
	table[0][map('$')] = "error";
	table[0][map('E')] = "1";
	table[0][map('T')] = "2";
	table[0][map('F')] = "3";
	table[1][map('+')] = "S6";
	table[1][map('-')] = "S7";
	table[1][map('*')] = "error";
	table[1][map('/')] = "error";
	table[1][map('1')] = "error";
	table[1][map('.')] = "error";
	table[1][map('(')] = "error";
	table[1][map(')')] = "error";
	table[1][map('$')] = "ACC";
	table[1][map('E')] = "error";
	table[1][map('T')] = "error";
	table[1][map('F')] = "error";
	table[2][map('+')] = "RE->T";
	table[2][map('-')] = "RE->T";
	table[2][map('*')] = "S8";
	table[2][map('/')] = "S9";
	table[2][map('1')] = "error";
	table[2][map('.')] = "error";
	table[2][map('(')] = "error";
	table[2][map(')')] = "RE->T";
	table[2][map('$')] = "RE->T";
	table[2][map('E')] = "error";
	table[2][map('T')] = "error";
	table[2][map('F')] = "error";
	table[3][map('+')] = "RT->F";
	table[3][map('-')] = "RT->F";
	table[3][map('*')] = "RT->F";
	table[3][map('/')] = "RT->F";
	table[3][map('1')] = "error";
	table[3][map('.')] = "error";
	table[3][map('(')] = "error";
	table[3][map(')')] = "RT->F";
	table[3][map('$')] = "RT->F";
	table[3][map('E')] = "error";
	table[3][map('T')] = "error";
	table[3][map('F')] = "error";
	table[4][map('+')] = "RF->num";
	table[4][map('-')] = "RF->num";
	table[4][map('*')] = "RF->num";
	table[4][map('/')] = "RF->num";
	table[4][map('1')] = "error";
	table[4][map('.')] = "S10";
	table[4][map('(')] = "error";
	table[4][map(')')] = "RF->num";
	table[4][map('$')] = "RF->num";
	table[4][map('E')] = "error";
	table[4][map('T')] = "error";
	table[4][map('F')] = "error";
	table[5][map('+')] = "error";
	table[5][map('-')] = "error";
	table[5][map('*')] = "error";
	table[5][map('/')] = "error";
	table[5][map('1')] = "S4";
	table[5][map('.')] = "error";
	table[5][map('(')] = "S5";
	table[5][map(')')] = "error";
	table[5][map('$')] = "error";
	table[5][map('E')] = "11";
	table[5][map('T')] = "2";
	table[5][map('F')] = "3";
	table[6][map('+')] = "error";
	table[6][map('-')] = "error";
	table[6][map('*')] = "error";
	table[6][map('/')] = "error";
	table[6][map('1')] = "S4";
	table[6][map('.')] = "error";
	table[6][map('(')] = "S5";
	table[6][map(')')] = "error";
	table[6][map('$')] = "error";
	table[6][map('E')] = "error";
	table[6][map('T')] = "12";
	table[6][map('F')] = "3";
	table[7][map('+')] = "error";
	table[7][map('-')] = "error";
	table[7][map('*')] = "error";
	table[7][map('/')] = "error";
	table[7][map('1')] = "S4";
	table[7][map('.')] = "error";
	table[7][map('(')] = "S5";
	table[7][map(')')] = "error";
	table[7][map('$')] = "error";
	table[7][map('E')] = "error";
	table[7][map('T')] = "13";
	table[7][map('F')] = "3";
	table[8][map('+')] = "error";
	table[8][map('-')] = "error";
	table[8][map('*')] = "error";
	table[8][map('/')] = "error";
	table[8][map('1')] = "S4";
	table[8][map('.')] = "error";
	table[8][map('(')] = "S5";
	table[8][map(')')] = "error";
	table[8][map('$')] = "error";
	table[8][map('E')] = "error";
	table[8][map('T')] = "error";
	table[8][map('F')] = "14";
	table[9][map('+')] = "error";
	table[9][map('-')] = "error";
	table[9][map('*')] = "error";
	table[9][map('/')] = "error";
	table[9][map('1')] = "S4";
	table[9][map('.')] = "error";
	table[9][map('(')] = "S5";
	table[9][map(')')] = "error";
	table[9][map('$')] = "error";
	table[9][map('E')] = "error";
	table[9][map('T')] = "error";
	table[9][map('F')] = "15";
	table[10][map('+')] = "error";
	table[10][map('-')] = "error";
	table[10][map('*')] = "error";
	table[10][map('/')] = "error";
	table[10][map('1')] = "S16";
	table[10][map('.')] = "error";
	table[10][map('(')] = "error";
	table[10][map(')')] = "error";
	table[10][map('$')] = "error";
	table[10][map('E')] = "error";
	table[10][map('T')] = "error";
	table[10][map('F')] = "error";
	table[11][map('+')] = "S6";
	table[11][map('-')] = "S7";
	table[11][map('*')] = "error";
	table[11][map('/')] = "error";
	table[11][map('1')] = "error";
	table[11][map('.')] = "error";
	table[11][map('(')] = "error";
	table[11][map(')')] = "S17";
	table[11][map('$')] = "error";
	table[11][map('E')] = "error";
	table[11][map('T')] = "error";
	table[11][map('F')] = "error";
	table[12][map('+')] = "RE->E+T";
	table[12][map('-')] = "RE->E+T";
	table[12][map('*')] = "S8";
	table[12][map('/')] = "S9";
	table[12][map('1')] = "error";
	table[12][map('.')] = "error";
	table[12][map('(')] = "error";
	table[12][map(')')] = "RE->E+T";
	table[12][map('$')] = "RE->E+T";
	table[12][map('E')] = "error";
	table[12][map('T')] = "error";
	table[12][map('F')] = "error";
	table[13][map('+')] = "RE->E-T";
	table[13][map('-')] = "RE->E-T";
	table[13][map('*')] = "S8";
	table[13][map('/')] = "S9";
	table[13][map('1')] = "error";
	table[13][map('.')] = "error";
	table[13][map('(')] = "error";
	table[13][map(')')] = "RE->E-T";
	table[13][map('$')] = "RE->E-T";
	table[13][map('E')] = "error";
	table[13][map('T')] = "error";
	table[13][map('F')] = "error";
	table[14][map('+')] = "RT->T*F";
	table[14][map('-')] = "RT->T*F";
	table[14][map('*')] = "RT->T*F";
	table[14][map('/')] = "RT->T*F";
	table[14][map('1')] = "error";
	table[14][map('.')] = "error";
	table[14][map('(')] = "error";
	table[14][map(')')] = "RT->T*F";
	table[14][map('$')] = "RT->T*F";
	table[14][map('E')] = "error";
	table[14][map('T')] = "error";
	table[14][map('F')] = "error";
	table[15][map('+')] = "RT->T/F";
	table[15][map('-')] = "RT->T/F";
	table[15][map('*')] = "RT->T/F";
	table[15][map('/')] = "RT->T/F";
	table[15][map('1')] = "error";
	table[15][map('.')] = "error";
	table[15][map('(')] = "error";
	table[15][map(')')] = "RT->T/F";
	table[15][map('$')] = "RT->T/F";
	table[15][map('E')] = "error";
	table[15][map('T')] = "error";
	table[15][map('F')] = "error";
	table[16][map('+')] = "RF->num.num";
	table[16][map('-')] = "RF->num.num";
	table[16][map('*')] = "RF->num.num";
	table[16][map('/')] = "RF->num.num";
	table[16][map('1')] = "error";
	table[16][map('.')] = "error";
	table[16][map('(')] = "error";
	table[16][map(')')] = "RF->num.num";
	table[16][map('$')] = "RF->num.num";
	table[16][map('E')] = "error";
	table[16][map('T')] = "error";
	table[16][map('F')] = "error";
	table[17][map('+')] = "RF->(E)";
	table[17][map('-')] = "RF->(E)";
	table[17][map('*')] = "RF->(E)";
	table[17][map('/')] = "RF->(E)";
	table[17][map('1')] = "error";
	table[17][map('.')] = "error";
	table[17][map('(')] = "error";
	table[17][map(')')] = "RF->(E)";
	table[17][map('$')] = "RF->(E)";
	table[17][map('E')] = "error";
	table[17][map('T')] = "error";
	table[17][map('F')] = "error";
	state.push_back(0);																	//��ʼ״̬S0��ջ
}

void printTable() {
	int i, j;
	printf("%18c%15c%15c%15c%15s%15c%15c%15c%15c%15c%15c%15c\n", '+', '-', '*', '/', "num", '.', '(',')', '$','E','T','F');
	cout << "   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	for (i = 0; i < 18; i++) {
		if (i < 10)
			printf("%d %c", i, '|');
		else
			printf("%d%c", i, '|');
		for (j = 0; j < 12; j++)
			printf("  %13s", table[i][j].c_str());
		printf("\n");
	}
}

void analyze(string input) {
	unsigned int j, k;
	double x, y;
	string tmp1, tmp2;
	string a, b;
	string::size_type index;
	stringstream stream;
	printf("ջ\t\t\t\t\t\t\t\t\t\t       ����\t\t\t\t\t\t\t\t\t���\n");
	while (table[state.back()][map(input[0])] != "ACC") {								//���������Ӧ�����"ACC"
		if (table[state.back()][map(input[0])] == "error") {							//���������Ӧ����Ϊerror
			cout << "State: ";
			for (j = 0; j < state.size(); j++)
				printf("%-2d ", state[j]);
			for (j = 3; j > state.size() / 3; j--)
				cout << "\t";
			printf("\t\t\t%35s   \t\t\t\t\t\t\t\t\t%-20s\n", input.c_str(), "error");	//���������Ϣ
			cout << "Symble:-  ";
			for (j = 0; j < stack.size(); j++)
				printf("%-2c ", stack[j]);
			cout << endl;
			cout << endl << "������Ŵ��������ķ����򣬷����������˳���" << endl << endl;
			return;																		//��������
		}
		else {																			//����
			index = table[state.back()][map(input[0])].find("S");
			if (index != string::npos) {												//���������Ӧ�����"S"��ִ���ƽ�����
				cout << "State: ";
				for (j = 0; j < state.size(); j++)
					printf("%-2d ", state[j]);
				for (j = 3; j > state.size() / 3; j--)
					cout << "\t";
				printf("\t\t\t%35s   \t\t\t\t\t\t\t\t\t%-20s\n", input.c_str(), table[state.back()][map(input[0])].c_str());
				cout << "Symble:-  ";
				for (j = 0; j < stack.size(); j++)
					printf("%-2c ", stack[j]);
				cout << endl;
				if (type.size()) 
					if (type.back() != "-") {
						cout << "Value:";
						printf("%.1f ", value.back());
						cout << "\t\t\t\tType:";
						printf("%s\n", (type.back()).c_str());
					}
				cout << endl;
				if (isDigit(input[0]) == 1) {											//���������Ϊ����
					stream.clear();
					stream << input[0];
					stream >> x;
					value.push_back(x);													//����ֵ������ջ
					type.push_back("-");
				}
				else {																	//����
					value.push_back(-1);
					type.push_back("-");
				}
				tmp1 = table[state.back()][map(input[0])];
				tmp1 = tmp1.substr(1, tmp1.length() - 1);
				stream.clear();
				stream << tmp1;
				stream >> k;
				state.push_back(k);														//�µ�״̬��״̬ջ
				stack.push_back(input[0]);												//������������ջ
				input = input.substr(1, input.length() - 1);							//��ǰָ��ǰ��
			}
			else {																		//���򣬸��ݷ������Ӧ����ִ�й�Լ����
				tmp1 = table[state.back()][map(input[0])];
				tmp2 = table[state.back()][map(input[0])];
				tmp2 = tmp2.substr(1, tmp2.length() - 1);
				index = tmp2.find("num");
				cout << "State: ";
				for (j = 0; j < state.size(); j++)
					printf("%-2d ", state[j]);
				for (j = 3; j > state.size() / 3; j--)
					cout << "\t";
				printf("\t\t\t%35s   \t\t\t\t\t\t\t\t\t%-20s\n", input.c_str(), tmp1.c_str());
				cout << "Symble:-  ";
				for (j = 0; j < stack.size(); j++)
					printf("%-2c ", stack[j]);
				cout << endl;
				if (type.size()) 
					if (type.back() != "-") {
						cout << "Value:";
						printf("%.1f ", value.back());
						cout << "\t\t\t\tType:";
						printf("%s\n", type.back().c_str());
					}
				cout << endl;
				if (index == string::npos) {
					for (j = 0; j < tmp2.length() - 3; j++) {
						state.pop_back();
						stack.pop_back();
					}
					if (tmp2 == "E->E+T") {												//����Լ���ò���ʽΪ"E->E+T"
						y = value.back();
						value.pop_back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						x = value.back();
						value.pop_back();
						x += y;
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						a = type.back();
						type.pop_back();												
						type.pop_back();
						b = type.back();
						type.pop_back();
						if (a == "real" || b == "real")
							type.push_back("real");
						else
							type.push_back("integer");
					}
					else if (tmp2 == "E->E-T") {										//����Լ���ò���ʽΪ"E->E-T"
						y = value.back();
						value.pop_back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						x = value.back();
						value.pop_back();
						x -= y;
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						a = type.back();
						type.pop_back();
						type.pop_back();
						b = type.back();
						type.pop_back();
						if (a == "real" || b == "real")
							type.push_back("real");
						else
							type.push_back("integer");
					}
					else if (tmp2 == "T->T*F") {										//����Լ���ò���ʽΪ"T->T*F"
						y = value.back();
						value.pop_back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						x = value.back();
						value.pop_back();
						x *= y;
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						a = type.back();
						type.pop_back();
						type.pop_back();
						b = type.back();
						type.pop_back();
						if (a == "real" || b == "real")
							type.push_back("real");
						else
							type.push_back("integer");
					}
					else if (tmp2 == "T->T/F") {										//����Լ���ò���ʽΪ"T->T/F"
						y = value.back();		
						value.pop_back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						x = value.back();
						value.pop_back();
						x /= y;
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						a = type.back();
						type.pop_back();
						type.pop_back();
						b = type.back();
						type.pop_back();
						if (a == "real" || b == "real")
							type.push_back("real");
						else
							type.push_back("integer");
					}
					else if (tmp2 == "F->(E)") {										//����Լ���ò���ʽΪ"F->(E)"
						value.pop_back();
						x = value.back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						value.pop_back();
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						type.pop_back();
						a = type.back();
						type.pop_back();
						type.pop_back();
						type.push_back(a);
					}
				}
				else {
					index = table[state.back()][map(input[0])].find("num.num");
					if (index == string::npos) {										//����Լ���ò���ʽΪ"F->num"
						for (j = 0; j < tmp2.length() - 5; j++) {
							state.pop_back();
							stack.pop_back();
						}
						type.pop_back();
						type.push_back("integer");
					}
					else {																//����Լ���ò���ʽΪ"F->num.num"
						for (j = 0; j < tmp2.length() - 7; j++) {
							state.pop_back();
							stack.pop_back();
						}
						y = value.back();
						value.pop_back();												//�Ҳ���������Ӧ������ֵ��ջ
						value.pop_back();
						x = value.back();
						value.pop_back();
						x += y / 10;
						value.push_back(x);												//�󲿷�������Ӧ������ֵ��ջ
						type.pop_back();
						type.pop_back();
						type.pop_back();
						type.push_back("real");
					}
				}
				stack.push_back(tmp2[0]);
				stream.clear();
				stream << table[state.back()][map(stack.back())];
				stream >> k;
				state.push_back(k);														//goto���Ӧ������״̬ջ
			}
		}
	}
	cout << "State: ";																	//����
	for (j = 0; j < state.size(); j++)
		printf("%-2d ", state[j]);
	for (j = 3; j > state.size() / 3; j--)
		cout << "\t";
	printf("\t\t\t%35s   \t\t\t\t\t\t\t\t\t%-20s\n", input.c_str(), "Accept");			//���������������ַ��������ķ�����
	cout << "Symble:-  ";
	for (j = 0; j < stack.size(); j++)
		printf("%-2c ", stack[j]);
	cout << endl;
	if (type.back() != "-") {
		cout << "Value:";
		printf("%.1f ", value.back());
		cout << "\t\t\t\tType:";
		printf("%s\n", type.back().c_str());
	}
	cout << endl;
}