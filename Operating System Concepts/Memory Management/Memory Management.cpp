#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;



HANDLE allo, trac;									//信号量的句柄



struct operation{
	int time;										//起始时间
	int block;										//内存页数
	int oper;										//操作
	int protection;									//权限
};
	
struct trace {										//跟踪每一次分配活动的数据结构
	LPVOID start;									//起始地址
	long size;										//分配的大小
};

DWORD Tracker(LPDWORD lpdwparm) {					//跟踪allocator线程的内存行为，并输出必要信息
	ofstream outfile;								//输出文件
	outfile.open("out.txt");

	for (int i = 0; i <= 30; i++){
		WaitForSingleObject(trac, INFINITE);		//等待allocator一次内存分配活动结束
		//打印内存状况和系统状况
		outfile << i << endl;
		
		SYSTEM_INFO info;							//系统信息
		GetSystemInfo(&info);
		outfile << "dwActiveProcessorMask" << '\t' << info.dwActiveProcessorMask << endl;
		outfile << "dwAllocationGranularity" << '\t' << info.dwAllocationGranularity << endl;
		outfile << "dwNumberOfProcessors" << '\t' << info.dwNumberOfProcessors << endl;
		outfile << "dwOemId" << '\t' << info.dwOemId << endl;
		outfile << "dwPageSize" << '\t' << info.dwPageSize << endl;
		outfile << "dwProcessorType" << '\t' << info.dwProcessorType << endl;
		outfile << "lpMaximumApplicationAddress" << '\t' << info.lpMaximumApplicationAddress << endl;
		outfile << "lpMinimumApplicationAddress" << '\t' << info.lpMinimumApplicationAddress << endl;
		outfile << "wProcessorArchitecture" << '\t' << info.wProcessorArchitecture << endl;
		outfile << "wProcessorLevel" << '\t' << info.wProcessorLevel << endl;
		outfile << "wProcessorRevision" << '\t' << info.wProcessorRevision << endl;
		outfile << "wReserved" << '\t' << info.wReserved << endl;
		outfile << "************************************************************************************" << endl;
		//内存状况
		MEMORYSTATUS status;						//内存状态
		GlobalMemoryStatus(&status);
		outfile << "dwAvailPageFile" << '\t' << status.dwAvailPageFile << endl;
		outfile << "dwAvailPhys" << '\t' << status.dwAvailPhys << endl;
		outfile << "dwAvailVirtual" << '\t' << status.dwAvailVirtual << endl;
		outfile << "dwLength" << '\t' << status.dwLength << endl;
		outfile << "dwMemoryLoad" << '\t' << status.dwMemoryLoad << endl;
		outfile << "dwTotalPageFile" << '\t' << status.dwTotalPageFile << endl;
		outfile << "dwTotalPhys" << '\t' << status.dwTotalPhys << endl;
		outfile << "dwTotalVirtual" << '\t' << status.dwTotalVirtual << endl;
		outfile << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;

		MEMORY_BASIC_INFORMATION mem;				//内存基本信息
		VirtualQuery(info.lpMinimumApplicationAddress, &mem,
			sizeof(MEMORY_BASIC_INFORMATION));
		outfile << "AllocationBase" << '\t' << mem.AllocationBase << endl;
		outfile << "AllocationProtect" << '\t' << mem.AllocationProtect << endl;
		outfile << "BaseAddress" << '\t' << mem.BaseAddress << endl;
		outfile << "Protect" << '\t' << mem.Protect << endl;
		outfile << "RegionSize" << '\t' << mem.RegionSize << endl;
		outfile << "State" << '\t' << mem.State << endl;
		outfile << "Type" << '\t' << mem.Type << endl;
		outfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		//释放信号量通知allocator可以执行下一次内存分配活动
		ReleaseSemaphore(allo, 1, NULL);
	}
	outfile.close();
	return 0;
}

void Allocator() {									//模拟内存分配活动的线程
	trace traceArray[5];
	int index = 0;
	FILE* file;
	file = fopen("opfile", "rb");					//读入文件
	operation op;
	SYSTEM_INFO info;
	DWORD temp;
	GetSystemInfo(&info);
	for (int i = 0; i<30; i++){
		WaitForSingleObject(allo, INFINITE);		//等待tracker打印结束的信号量
		cout << i << ':';
		fread(&op, sizeof(operation), 1, file);
		Sleep(op.time);								//执行时间
		GetSystemInfo(&info);
		switch (op.protection) {					//根据文件内容确定权限
			case 0:
				index = 0;
				temp = PAGE_READONLY;
				break;
			case 1:
				temp = PAGE_READWRITE;
				break;
			case 2:
				temp = PAGE_EXECUTE;
				break;
			case 3:
				temp = PAGE_EXECUTE_READ;
				break;
			case 4:
				temp = PAGE_EXECUTE_READWRITE;
				break;
			default:
				temp = PAGE_READONLY;
		}
		switch (op.oper){
			case 0:									//保留一个区域
				cout << "reserve now" << endl;

				traceArray[index].start = VirtualAlloc(NULL, op.block*info.dwPageSize,MEM_RESERVE, PAGE_NOACCESS);
				traceArray[index++].size = op.block*info.dwPageSize;
				cout << "starting address:"
					<< traceArray[index - 1].start << '\t' << "size:" << traceArray[index - 1].size << endl;
				break;
			case 1:									//提交一个区域
				cout << "commit now" << endl;

				traceArray[index].start = VirtualAlloc(traceArray[index].start, traceArray[index].size, MEM_COMMIT, temp);
				index++;
				cout << "starting address:"
					<< traceArray[index - 1].start << '\t' << "size:" << traceArray[index - 1].size << endl;
				break;
			case 2:									//锁一个区域
				cout << "lock now" << endl;
				cout << "starting address:" << traceArray[index].start << '\t' << "size:" << traceArray[index].size << "\t";
				if (!VirtualLock(traceArray[index].start, traceArray[index++].size)) {
					cout << GetLastError();
					if (GetLastError() == 998)
						cout << " - 内存分配访问无效";
				}
				cout << endl;
				break;
			case 3:									//解锁一个区域
				cout << "unlock now" << endl;
				cout << "starting address:" << traceArray[index].start << '\t' << "size:" << traceArray[index].size << "\t";
				if (!VirtualUnlock(traceArray[index].start, traceArray[index++].size)) {
					cout << GetLastError();
					if (GetLastError() == 158)
						cout << " - 段已解除锁定";
				}
				cout << endl;
				break;
			case 4:									//回收一个区域
				cout << "decommit now" << endl;
				cout << "starting address:" << traceArray[index].start << '\t' << "size:" << traceArray[index].size << "\t";
				if (!VirtualFree(traceArray[index].start, traceArray[index++].size, MEM_DECOMMIT)) {
					cout << GetLastError();
					if (GetLastError() == 87)
						cout << " - 参数错误";
				}
				cout << endl;
				break;
			case 5:									//释放一个区域
				cout << "release now" << endl;
				cout << "starting address:" << traceArray[index].start << '\t' << "size:" << traceArray[index].size << "\t";
				if (!VirtualFree(traceArray[index++].start, 0, MEM_RELEASE))
					cout << GetLastError();
				cout << endl;
				break;
			default:
				cout << "error" << endl;
		}
		ReleaseSemaphore(trac, 1, NULL);			//释放信号量通知tracker可以打印信息
	}
	fclose(file);
}

int main(){
	FILE* file;
	file = fopen("opfile", "wb");					//“opfile”为二进制用以确定内存操作
	operation op;
	DWORD dwThread;
	HANDLE handle[2];

	for (int j = 0; j<6; j++) //0-保留；1-提交；2-锁；3-解锁；4-回收；5-释放
		for (int i = 0; i<5; i++){
			//0-PAGE_READONLY;
			//1-PAGE_READWRITE;
			//2-PAGE_EXECUTE;
			//3-PAGE_EXECUTE_READ;
			//4-PAGE_EXECUTE_READWRITE;
			op.time = rand() % 1000;				//随机生成等待时间
			op.block = rand() % 5 + 1;				//随机生成块大小
			op.oper = j;
			op.protection = i;
			fwrite(&op, sizeof(operation), 1, file);//将生成的结构写入文件
		}
	fclose(file);
	//生成两个线程
	handle[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Tracker, NULL, 0, &dwThread);
	handle[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Allocator, NULL, 0, &dwThread);
	//生成两个信号量
	allo = CreateSemaphore(NULL, 0, 1, L"allo");
	trac = CreateSemaphore(NULL, 1, 1, L"trac");
	//等待线程执行的执行结束后，再退出
	WaitForMultipleObjects(2, handle, TRUE, INFINITE);
	system("pause");
	return 0;
}