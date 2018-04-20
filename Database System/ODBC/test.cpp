#include <iostream>      
#include <string>      
#include <windows.h> 
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>      
#include <odbcss.h>
using namespace std;

#pragma comment (lib,"odbc32.lib")
#pragma comment (lib,"libmysql.lib")
#define MAXBUFLEN 255  

/*初始化 ODBC*/
SQLHENV henv = SQL_NULL_HENV;					//初始化环境句柄
SQLHDBC hdbc = SQL_NULL_HDBC;					//初始化连接句柄
SQLHSTMT hstmt = SQL_NULL_HSTMT;				//初始化语句句柄

struct Course{
	char cno[4];
	char cname[15];
	int credit;
};

struct Student{
	char sno[7];
	char sname[7];
	char sex[3];
	char bdate[20];
	char dept[9];
	char classno[5];
};

struct Sc{
	char sno[7];
	char cno[4];
	int grade;
};

unsigned char sqlSelect[3][100] = { "select * from course", "select * from student", "select * from sc" };
unsigned char sqlOper[MAXBUFLEN];

void priCourse(HDBC);
void priStudent(HDBC);
void priSc(HDBC);

int main(){
	RETCODE retcode;							//定义返回代码
	UCHAR szDSN[10] = "mysqlodbc";				//数据源
	UCHAR userID[5] = "root";					//用户名
	UCHAR passWORD[7] = "142857";				//密码
	string s;
  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	retcode = SQLConnect(hdbc, szDSN, SQL_NTS, userID, SQL_NTS, passWORD, SQL_NTS);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		cout << "Connect failed!" << endl;
	else 
		while (true) {
			cout << "mysql>";
			getline(cin, s);
			if (s != "exit") {
				strcpy_s((char*)sqlOper, MAXBUFLEN, s.c_str());

				retcode = SQLAllocStmt(hdbc, &hstmt);
				retcode = SQLExecDirect(hstmt, sqlOper, SQL_NTS);

				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
					cout << "Query OK(0.00 sec)" << endl;
				else
					cout << "ERROR" << endl;
				SQLFreeStmt(hstmt, SQL_DROP);

				if (s.find("course") != string::npos)
					priCourse(hdbc);
				else if (s.find("student") != string::npos)
					priStudent(hdbc);
				else if (s.find("sc") != string::npos)
					priSc(hdbc);
			}
			else {
				cout << "Bye~" << endl;
				break;
			}
		}
	system("pause");
	return 0;
}

void priCourse(HDBC hdbc){
	HSTMT hstmt;
	RETCODE retcode;

	long lenOut1, lenOut2, lenOut3;
	struct Course* ccell;
	ccell = (struct Course*)malloc(sizeof(struct Course));

	retcode = SQLAllocStmt(hdbc, &hstmt);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
		retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, ccell->cno, 4, &lenOut1);
		retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, ccell->cname, 15, &lenOut2);
		retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &ccell->credit, sizeof(int), &lenOut3);

		retcode = SQLExecDirect(hstmt, sqlSelect[0], SQL_NTS);
		
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
			cout << "Course is as following:" << endl;
			cout << "*******************************" << endl;
			cout << "cno       cname          credit" << endl;

			retcode = SQLFetch(hstmt);
			
			while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
				printf("%-10s%-20s%-16d\n", ccell->cno, ccell->cname, ccell->credit);
				retcode = SQLFetch(hstmt);
			}
			cout << "*******************************" << endl;
		}
	}
	SQLFreeStmt(hstmt, SQL_DROP);
}

void priStudent(HDBC hdbc){
	HSTMT hstmt;
	RETCODE retcode;

	long lenOut1, lenOut2, lenOut3, lenOut4, lenOut5, lenOut6;
	struct Student* scell;
	scell = (struct Student*)malloc(sizeof(struct Student));

	retcode = SQLAllocStmt(hdbc, &hstmt);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
		retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, scell->sno, 7, &lenOut1);
		retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, scell->sname, 7, &lenOut2);
		retcode = SQLBindCol(hstmt, 3, SQL_C_CHAR, &scell->sex, 3, &lenOut3);
		retcode = SQLBindCol(hstmt, 4, SQL_C_CHAR, &scell->bdate, 20, &lenOut4);
		retcode = SQLBindCol(hstmt, 5, SQL_C_CHAR, scell->dept, 9, &lenOut5);
		retcode = SQLBindCol(hstmt, 6, SQL_C_CHAR, scell->classno, 5, &lenOut6);

		retcode = SQLExecDirect(hstmt, sqlSelect[1], SQL_NTS);
		
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
			cout << "Student is as following:" << endl;
			cout << "*****************************************************************************" << endl;
			cout << "sno       sname          sex       bdate                    dept      classno" << endl;

			retcode = SQLFetch(hstmt);
			
			while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
				printf("%-10s%-15s%-10s%-25s%-10s%-15s\n", scell->sno, scell->sname, scell->sex, scell->bdate, scell->dept, scell->classno);
				retcode = SQLFetch(hstmt);
			}
			cout << "*****************************************************************************" << endl;
		}
	}
	SQLFreeStmt(hstmt, SQL_DROP);
}

void priSc(HDBC hdbc) {
	HSTMT hstmt;
	RETCODE retcode;

	long lenOut1, lenOut2, lenOut3;
	struct Sc* cell;
	cell = (struct Sc*)malloc(sizeof(struct Sc));

	retcode = SQLAllocStmt(hdbc, &hstmt);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, cell->sno, 7, &lenOut1);
		retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, cell->cno, 4, &lenOut2);
		retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &cell->grade, sizeof(int), &lenOut3);

		retcode = SQLExecDirect(hstmt, sqlSelect[2], SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			cout << "Sc is as following:" << endl;
			cout << "****************************" << endl;
			cout << "sno       cno          grade" << endl;

			retcode = SQLFetch(hstmt);

			while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				printf("%-10s%-16s%d\n", cell->sno, cell->cno, cell->grade);
				retcode = SQLFetch(hstmt);
			}
			cout << "****************************" << endl;
		}
	}
	SQLFreeStmt(hstmt, SQL_DROP);
}