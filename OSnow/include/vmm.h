#pragma once
#include<vector>
#include<unordered_map>
#include <cassert>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include "fs_head.h"

/*
* some define.
*/
#define DEBUG 1
#define RELEASE 1
#define PAGE_SIZE 4096
#define MAX_MEMORY_SIZE 16614178816
#define ERROR_ADDR 0
#define REPLACEMENT_ALGORITHM SECOND_CHANCE_ALGORITHM
#define LFU 1
#define SECOND_CHANCE_ALGORITHM 2
#define MFU_ALGORITHM 3


#if RELEASE
extern std::fstream flog;
#endif // RELEASE



typedef unsigned __int32 vaddr;
typedef unsigned __int32 paddr;
typedef __int16 flag;
typedef __int16 procid;
typedef struct PTE PTE;
typedef struct VMM VMM;
typedef vaddr size_vm;
typedef  std::string disk_word;
typedef __int32 INT;
typedef unsigned long vtime;


/*
*	Each process have a VMM struct to keep its virtual memory infomations.
*/
struct VMM
{
	procid pid;
	vaddr begin_vaddr;
	unsigned  int  page_number;
	struct VMM *pnxt;
	VMM(procid _begin_vaddr, vaddr _page_number) :begin_vaddr(_begin_vaddr), page_number(_page_number)
		, pnxt(NULL)
	{
		assert(0 == _begin_vaddr%PAGE_SIZE);
	}
	VMM() {};
};



/*
* The pte status.
*/
#define VALUABLE 1 //the pte is available
#define ILLEGAL 0  //the pte is illegal
#define MOTIFIED 3  // the pte is motified
/*
* Page Table Entity
*/
struct PTE
{
	// the process id
	procid id;
	// the virtual memory address
	vaddr vaddress;
	// the physical memory address
	paddr paddress;
	// the status
	flag  status;
	// define a count for replace algorithm.
	INT count;
	// define the handle of the disk.
	disk_word disk;
	// define the arrive time of this pte.
	vtime arr_time;
	// shared memory;
	std::vector<procid> shared_id;
	PTE(procid _id, vaddr _vaddress):id(_id),vaddress(_vaddress),paddress(0),status(ILLEGAL)
	{
		assert(0 == _vaddress%PAGE_SIZE);
	}
};


/*
*	Allocate somevirtual memory to VMM.
*/
vaddr Allocate_VM(const procid &id, size_vm size);

/*
*	Free all virtual memory.
*/
int Free_VM(const procid &id);

/*
* Write some data into virtual memory.
*/
int Write_VM(const procid &id, vaddr begin_vaddr, size_vm size, char * content);

/*
*	Read some data from virtual memory.
*/
int Read_VM(const procid &id, vaddr begin_vaddr, size_vm size, char *content);

/*
* copy the memory when fork a process.
*/
int fork_memory(procid parent,procid child);







/*
translate virtual address into physical address.
use by user process.
*/
paddr vaddr2paddr(const procid &id, vaddr vaddress);

/*
translate virtual address into physical address.
use by kerrnel.
*/
paddr vaddr2paddr(vaddr vaddress);

/*
*	I forget the useage of this function.
*/
int Motify_PTE(const procid &id, vaddr vaddress);

/*
translate physical address into virtual address.
use by kerrnel.
*/
//vaddr paddr2vaddr(paddr paddress);

/*
*	Get the status of a pte.
*/
flag Get_PTE_statues(const procid &id, vaddr vaddress);

/*
*	Get the VMM by process id.
*/
VMM * GetVMM(const procid &id);

/*
* Init all VMM.
*/
int VMInit();

/*
*	replace function.
*/
paddr ReplacePM(const procid &id, PTE* source_pte);

/*
*	Get the phisical memory.
*	The interface of pm.
*/
char Get_PM(paddr paddress);

/*
*	Get the pte,
*/
PTE * GetPTE(const procid &id, vaddr vaddress);

/*
*	Write to phisical memory.
*	The interface of pm.
*/
int Write_PM(paddr paddress, char c);

/*
*	Show the usage of the pm.
*/
void ShowPTE(const procid &id);

/*
*	Write to Disk.
*	The interface of disk.
*/
int Write_Disk(PTE * pte);

/*
*	Load pm from Disk.
*	The interface of disk.
*/
int Load_Disk(PTE * pte);

/*
*	Get a pte.
*/
std::vector<PTE*> Get_Proc_PTE(const procid &id);

/*
*	LFU replacement algorithm.
*/
PTE * LFU_Algorithm(const procid &id);

/*
*	Second chance replacement algorithm.
*/
PTE * Second_Chance_Algorithm(const procid &id);

extern std::vector<PTE*> page_table;
extern std::vector<VMM*> proc_vmm;
