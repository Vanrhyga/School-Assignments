#include<iostream>
#include <cassert>

#include"vmm.h"
#include"fs_head.h"
#include"mmu.h"
//static unsigned long count_time = 0;
static unsigned long count_time = 0;

std::vector<PTE*> page_table;
std::vector<VMM*> proc_vmm;


#if RELEASE
std::fstream flog;
#endif // RELEASE

paddr vaddr2paddr(  procid &id, vaddr vaddress)
{
	for (PTE *pte : page_table)
	{
		if (id == pte->id)
		{
			if (vaddress >= pte->vaddress && vaddress < pte->vaddress + PAGE_SIZE)
			{
				// this page is not in pm
				if (pte->status == ILLEGAL)
				{
					//repleacemet algorithm
					ReplacePM(id, pte);
					//std::cout << "From PTE    ";
					//std::cout << "pid:" << id << "\t\t\tvaddr: " << pte->vaddress << "\t\t\tpaddr:" << pte->paddress << "\t\t\tstatus:  " << pte->status << "\n";
					return paddr(vaddress - pte->vaddress + pte->paddress);
				}
				// in pm
				else
				{
					//std::cout << "From PTE    ";
					//std::cout << "pid:" << id << "\t\t\tvaddr: " << pte->vaddress << "\t\t\tpaddr:" << pte->paddress << "\t\t\tstatus:  " << pte->status << "\n";
					return paddr(vaddress - pte->vaddress + pte->paddress);
				}
			}
		}
	}
	return paddr(ERROR_ADDR);
}

paddr vaddr2paddr(vaddr vaddress)
{
	for (PTE *pte : page_table)
	{
		if (vaddress >= pte->vaddress && vaddress < pte->vaddress + PAGE_SIZE)
			return paddr(vaddress - pte->vaddress + pte->paddress);
	}
	return paddr(ERROR_ADDR);
}

int Motify_PTE(  procid &id, vaddr vaddress)
{
	for (PTE *pte : page_table)
	{
		if (id == pte->id)
		{
			if (vaddress >= pte->vaddress && vaddress < pte->vaddress + PAGE_SIZE)
			{
				// this page is in pm
				if (pte->status != ILLEGAL)
				{
					pte->status = MOTIFIED;
					return 1;
				}
			}
		}
	}
	return -1;
}

void ShowPTE(  procid &id)
{
#if DEBUG
	std::cout << "=====================================pid: " << id << "====================================\n";
	std::cout << "Show PTE\n";
	for (PTE* pte : page_table)
	{
		if (pte->id == id)
		{
			std::cout << "pid:" << id << "\t\tvaddr: " << pte->vaddress << "\t\tpaddr:" << pte->paddress << "\t\tstatus:  " << ((pte->status == VALUABLE) ? "V" : "I") << "\t\tarr_time:  " << pte->arr_time << "\t\tcount:  " << pte->count << "\n";
		}
	}
	std::cout << "Show end\n";
	std::cout << "============================================================================\n";

#endif // DEBUG
#if RELEASE
	flog << "============================================================================\n";
	for (PTE* pte : page_table)
	{
		if (pte->id == id)
		{
			flog << "pid:" << id << "\t\tvaddr: " << pte->vaddress << "\t\tpaddr:" << pte->paddress << "\t\tstatus:  " << ((pte->status == VALUABLE) ? "V" : "I") << "\t\tarr_time:  " << pte->arr_time << "\t\tcount:  " << pte->count << "\n";
		}
	}
	flog << "============================================================================\n";

#endif // RELEASE

}
#include<sstream>
int Write_Disk(PTE* pte)
{
	std::stringstream ss;
	ss << pte->vaddress;
	std::string tmp_string = ss.str();
	pte->disk = pte->id + tmp_string;
	char* tmp = new char[pte->disk.length() + 1];
	strcpy(tmp, pte->disk.c_str());
	char pm[PAGE_SIZE];
	for (size_t i = 0; i < PAGE_SIZE; i++)
	{
		pm[i] = Get_PM(pte->paddress + i);
	}
	int fuck[3] = { 1,1,1 };
	if (createFile(tmp, fuck) && openFile(tmp) && writeFile_auto(tmp, pm) && closeFile_auto(tmp))
	{
		return 1;
	}
	return 0;
}

int Load_Disk(PTE* pte)
{
	if (pte->disk.empty())
	{
		return 0;
	}
	char* file_name = new char[pte->disk.length() + 1];
	strcpy(file_name, pte->disk.c_str());

	char *tmp = nullptr;
	if (openFile(file_name))
	{
		tmp = readFile_auto(file_name);
		int i = 0;
		while (tmp && i++ < PAGE_SIZE)
		{
			Write_PM(pte->paddress, *(tmp + i));
		}
		closeFile_auto(file_name);
		deleteFile(file_name);
		return 1;
	}
	return 0;
}

std::vector<PTE*> Get_Proc_PTE(  procid &id)
{
	std::vector<PTE*> temp;
	std::vector<PTE*>::iterator it;
	//for(it =  page_table.begin();it != page_table.end();i++)
	for (PTE *pte : page_table)
	{
		if (id == pte->id)
		{
			PTE* tmp_pte = pte;
			temp.push_back(tmp_pte);
		}
	}
	return temp;
}

PTE* LFU_Algorithm(  procid &id)
{
	//std::vector<PTE*> temp = Get_Proc_PTE(id);
	int temp_c = INT_MAX;
	PTE* temp_pte = nullptr;
	//for (PTE* pte : temp)
	for (PTE* pte : page_table)
	{
		if (pte->count < temp_c && pte->status != ILLEGAL && pte->id == id)
		{
			temp_c = pte->count;
			temp_pte = pte;
		}
	}
	temp_pte->count = 0;
	temp_pte->status = ILLEGAL;
	return temp_pte;
}

PTE* Second_Chance_Algorithm(  procid &id)
{
	std::vector<PTE*> temp = Get_Proc_PTE(id);
	vtime temp_t = ULONG_MAX;
	PTE* temp_pte = nullptr, *earliest_pte = nullptr;


	while (nullptr == temp_pte)
	{
		temp_t = ULONG_MAX;
		/*find the earilest.*/
		for (PTE* pte : temp)
		{
			if (pte->arr_time < temp_t && pte->status != ILLEGAL)
			{
				temp_t = pte->arr_time;
				temp_pte = pte;
			}
		}
		if (temp_pte == nullptr)
		{
			temp_pte = earliest_pte;
			break;
		}
		if (temp_pte->count % 2 == 1)
		{
			temp_pte->count--;
			if (earliest_pte == nullptr)
			{
				earliest_pte = temp_pte;
			}

			for (size_t i = 0; i < temp.size(); i++)
			{
				if (temp[i] == temp_pte)
				{
					temp.erase(temp.begin() + i);
					break;
				}
			}
			temp_pte = nullptr;
		}
		else
		{
			break;
		}
	}
	temp_pte->count = 0;
	temp_pte->status = ILLEGAL;
	return temp_pte;
}

PTE* MFU_Algorithm(  procid &id)
{
	std::vector<PTE*> temp = Get_Proc_PTE(id);
	int temp_c = -1;
	PTE* temp_pte = nullptr;
	for (PTE* pte : temp)
	{
		if (pte->count > temp_c && pte->status != ILLEGAL)
		{
			temp_c = pte->count;
			temp_pte = pte;
		}
	}
	temp_pte->count = 0;
	temp_pte->status = ILLEGAL;
	return temp_pte;
}

paddr ReplacePM(  procid &id, PTE* source_pte)
{
	PTE* (*replacement_algorithm)(  procid &) = nullptr;
	switch (REPLACEMENT_ALGORITHM)
	{
	case LFU:
	{
		replacement_algorithm = LFU_Algorithm;
		break;
	}
	case SECOND_CHANCE_ALGORITHM:
	{
		replacement_algorithm = Second_Chance_Algorithm;
		break;
	}
	case MFU_ALGORITHM:
	{
		replacement_algorithm = MFU_Algorithm;
		break;
	}
	default:
	{
		replacement_algorithm = LFU_Algorithm;
		break;
	}
	}
	PTE* victim = replacement_algorithm(id);
	if (victim == nullptr)
	{
		std::cerr << "victim could not be null.\n";
	}
	paddr temp = victim->paddress;
	Write_Disk(victim);
	source_pte->paddress = temp;
	source_pte->count = 0;
	source_pte->arr_time = count_time++;
	source_pte->status = VALUABLE;
	Load_Disk(source_pte);
#if DEBUG
	std::cout << "Page replace: victim(" << victim->vaddress << ")  \n";
	ShowPTE(id);
#endif // DEBUG
#if RELEASE
	flog << count_time << "\tPage replace: source(" << source_pte->vaddress << ")\t" << " victim(" << victim->vaddress << ")  \n";
#endif // RELEASE


	return paddr(source_pte->paddress);
}

char Get_PM(paddr paddress)
{
	return read_pm(paddress);
	return 'c';
}

PTE* GetPTE(  procid &id, vaddr vaddress)
{
	for (vector<PTE *>::const_iterator iter = page_table.cbegin(); iter != page_table.cend(); iter++)
	{
		if (id == (*iter)->id)
		{
			if (vaddress >= (*iter)->vaddress && vaddress < (*iter)->vaddress + PAGE_SIZE)
			{
				PTE* tmp = (*iter);
				return tmp;
			}
		}
	}
	//	for (PTE *pte : page_table)
	//{
	//	if (id == pte->id)
	//	{
	//		if (vaddress >= pte->vaddress && vaddress < pte->vaddress + PAGE_SIZE)
	//		{
	//			return pte;
	//		}
	//	}
	//}
	return nullptr;

}

int Write_PM(paddr paddress, char c)
{
	write_pm(paddress, c);
	return 1;
}

int Write_VM(  procid &id, vaddr begin_vaddr, size_vm size, char * content)
{
	try
	{
		count_time++;
		size_vm page_num = size / PAGE_SIZE + 1, i = 0;
		/*
		begin vm address.
		*/
		vaddr begin_page_addr = (begin_vaddr / PAGE_SIZE)*PAGE_SIZE;

		while (page_num--)
		{
			paddr begin_pm_addr = vaddr2paddr(id, begin_vaddr);
			if (ERROR_ADDR == begin_page_addr)
			{
				return -1;
			}
			/*
			write
			*/
			for (size_vm j = 0; j < PAGE_SIZE && j < size; j++)
			{
				Write_PM(begin_pm_addr + j, content[i++]);
				GetPTE(id, begin_vaddr)->count += 2;
			}

			/*
			motify
			*/
			//Motify_PTE(id, begin_vaddr);
			begin_vaddr += PAGE_SIZE;
		}
#if DEBUG
		std::cout << "Write Successfully from " << begin_vaddr << " to " << begin_vaddr + size << "\n";
#endif // DEBUG
#if RELEASE
		flog << count_time << "\tWrite  from vm(" << begin_vaddr << ") to vm(" << begin_vaddr + size << ")\n";
#endif // RELEASE


		return 0;
	}
	catch (...)
	{
		std::cout << "write vm error \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
}

int Read_VM(  procid &id, vaddr begin_vaddr, size_vm size, char *content)
{
	try
	{
		count_time++;
		size_vm page_num = size / PAGE_SIZE + 1, i = 0;

		/*
		begin vm address.
		*/
		vaddr begin_page_addr = (begin_vaddr / PAGE_SIZE)*PAGE_SIZE;

		while (page_num--)
		{
			paddr begin_pm_addr = vaddr2paddr(id, begin_vaddr);
			if (ERROR_ADDR == begin_pm_addr)
			{
				return -1;
			}
			for (size_vm j = 0; j < PAGE_SIZE && j < size; j++)
			{
				//content[i++] = Get_PM(begin_pm_addr+j);
				PTE* tmp_pte = GetPTE(id, begin_vaddr);
				if (tmp_pte == nullptr)
				{
					return -1;
				}
				if (GetPTE(id, begin_vaddr)->count % 2 == 0)
				{
					GetPTE(id, begin_vaddr)->count += 3;
				}
				else
				{
					GetPTE(id, begin_vaddr)->count += 2;
				}
			}
			begin_vaddr += PAGE_SIZE;
			size -= PAGE_SIZE;
		}
#if DEBUG
		std::cout << "Read Successfully from " << begin_vaddr << " to " << begin_vaddr + size << "\n";
#endif // DEBUG
#if RELEASE
		flog << count_time << "\tRead  from vm(" << begin_vaddr << ") to vm(" << begin_vaddr + size << ")\n";
#endif // RELEASE

		return 0;
	}
	catch (...)
	{
		std::cout << "read vm error \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
}

vaddr fork_memory(procid parent, procid child)
{
	try 
	{
		size_t page_count = GetVMM(parent)->page_number;
		Allocate_VM(child, page_count*PAGE_SIZE);
		char* buffer = new char[PAGE_SIZE*page_count + 1];
		VMM* src = GetVMM(parent), *dst = GetVMM(child);
		Read_VM(parent, src->begin_vaddr, page_count*PAGE_SIZE, buffer);
		Write_VM(child, dst->begin_vaddr, page_count*PAGE_SIZE, buffer);
		return dst->begin_vaddr;
	}
	catch (...)
	{
		std::cout << "fork vm error \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
}

flag Get_PTE_statues(  procid &id, vaddr vaddress)
{
	for (PTE *pte : page_table)
	{
		if (id == pte->id)
		{
			if (vaddress >= pte->vaddress && vaddress < pte->vaddress + PAGE_SIZE)
			{
				return pte->status;
			}
		}
	}
	return flag(ILLEGAL);
}

VMM* GetVMM(  procid &id)
{
	for (vector<VMM*>::const_iterator iter = proc_vmm.cbegin(); iter != proc_vmm.cend(); iter++)
	{
		if (id == (*iter)->pid)
		{
			VMM* tmp = (*iter);
			return tmp;
		}
	}
	for (VMM* vmm : proc_vmm)
	{
		if (vmm->pid == id)
		{
			return vmm;
		}
	}
	return nullptr;
}

vaddr Allocate_VM(  procid &id, size_vm size)
{
	try
	{
		//char* c = NULL;
		//*c = 0;
		/*if out of max size then exit.*/
		if (size >  MAX_MEMORY_SIZE)
		{
			return ERROR_ADDR;
		}
		VMM *pvmm = nullptr;
		pvmm = GetVMM(id);

		if (pvmm == nullptr)
		{
			//std::cout << "process is not running.\n";
			pvmm = new VMM;
			pvmm->pid = id;
			pvmm->begin_vaddr = PAGE_SIZE;
			pvmm->page_number = 0;
			proc_vmm.push_back(pvmm);
			//return ERROR_ADDR;
		}
		/*lack some code.*/
		/*to get the finally address*/
		vaddr _fvaddr = pvmm->begin_vaddr + pvmm->page_number*PAGE_SIZE, _begin_addr;
		assert(0 == _fvaddr%PAGE_SIZE);
		_begin_addr = _fvaddr;


		/*modify the vmm*/
		size_vm page_number = size / PAGE_SIZE + 1;
		pvmm->page_number += page_number;


		/*
		pm count
		*/
		int pm_cout = 3;

		int tmp = alloc(id, PAGE_SIZE*page_number);
		if (tmp == -1)
			return 0;
		/*insert the page table*/
		while (page_number--)
		{
			PTE *pte = new PTE(id, _fvaddr);
			pte->arr_time = count_time++;
			pte->count = 0;
			if (pm_cout)
			{
				pm_cout--;
				//pte->paddress = _fvaddr + 4096;  // this is algorithm is only for test
				pte->paddress = tmp;
				pte->status = VALUABLE;
			}
			page_table.push_back(pte);
			_fvaddr += PAGE_SIZE;
		}
#if DEBUG
		std::cout << "vaddress is " << _begin_addr << " to " << _begin_addr + size << "\n";
		ShowPTE(id);
#endif // DEBUG
#if RELEASE
		flog << count_time << "\tAllocate pid(" << id << ") " << "vaddress is " << _begin_addr << " to " << _begin_addr + size << "\n";
#endif // RELEASE


		return _begin_addr;
	}
	catch (...)
	{
		std::cout << "allocate vm error \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}

}

int Free_VM(  procid &id)
{
	try
	{

		VMM *pvmm = nullptr;
		pvmm = GetVMM(id);
		if (pvmm == nullptr)
		{
			std::cout << "process is not running.\n";
			return ERROR_ADDR;
		}
		free(id);
		//for (size_t i = page_table.size()-1; i >= 0 ; i--)
		//{
		//if (page_table[i]->id == id)
		//{
		//	PTE* temp = page_table[i];
		//	if (temp->status == VALUABLE)
		//	{
		//		free(temp->paddress);
		//	}
		//	page_table.erase(page_table.begin()+i);
		//	delete temp;
		//	if (page_table.empty())
		//	{
		//		break;
		//	}
		//	/*
		//	lack some codes for remove
		//	*/
		//}
		//}

		//delete pvmm;

#if DEBUG
		std::cout << "Memory Free " << id << "\n";
#endif // DEBUG
#if RELEASE
		flog << count_time << "\tMemory Free pid(" << id << ")\n";
#endif // RELEASE


		return 1;
	}
	catch (...)
	{
		std::cout << "free vm error \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}

}

int VMInit()
{
	/*clear the page table*/
	page_table.clear();

#if DEBUG
	std::cout << "initialization successfully.\n";
#endif //

#if RELEASE
	flog.open("vm_log.log", std::ios::app);
#endif // RELEASE

	return 0;
}