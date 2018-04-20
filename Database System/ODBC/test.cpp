#include <stdio.h>      
#include <string.h>      
#include <windows.h> 
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>      
#include <odbcss.h>  

#pragma comment (lib,"odbc32.lib")
#pragma comment (lib,"libmysql.lib")
#define MAXBUFLEN 255  
SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc = SQL_NULL_HDBC;
SQLHSTMT hstmt = SQL_NULL_HSTMT;

struct course{
	char cno[4];
	char cname[15];
	int credit;
};

unsigned char sqlSelect[3][100] = { "select * from course", "select * from student", "select * from sc" };

void showCourse(HDBC);

int main(){
	RETCODE retcode;
	UCHAR szDSN[10] = "mysqlodbc";  
	UCHAR userID[5] = "root";  
	UCHAR passWORD[7] = "142857"; 
  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	retcode = SQLConnect(hdbc, szDSN, SQL_NTS, userID, SQL_NTS, passWORD, SQL_NTS);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		printf("连接失败!\n");
	else {
		printf("连接成功!\n");
		showCourse(hdbc);
	}
	system("pause");
	return 0;
}

void showCourse(HDBC hdbc){
	HSTMT hstmt;
	RETCODE retcode;

	long lenOut1, lenOut2, lenOut3;
	struct course* ccell;
	ccell = (struct course*)malloc(sizeof(struct course));

	retcode = SQLAllocStmt(hdbc, &hstmt);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
		retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, ccell->cno, 4, &lenOut1);
		retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, ccell->cname, 15, &lenOut2);
		retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &ccell->credit, sizeof(int), &lenOut3);

		retcode = SQLExecDirect(hstmt, sqlSelect[0], SQL_NTS);
		
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
			printf("Course is as following:\n");
			printf("----------------------------------------------------------------\n");
			printf("cno       cname          credit\n");

			retcode = SQLFetch(hstmt);
			
			while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){
				printf("%s\t%s\t%d\n", ccell->cno, ccell->cname, ccell->credit);
				retcode = SQLFetch(hstmt);
			}
			printf("----------------------------------------------------------------\n\n");
		}
	}
	SQLFreeStmt(hstmt, SQL_DROP);
}
