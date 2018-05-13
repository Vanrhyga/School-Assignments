#ifndef FS_HEAD_H_INCLUDED
#define FS_HEAD_H_INCLUDED

#include<stdio.h>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
//#include<unistd.h>
#include<direct.h>
//#include "vmm.h"

#define True 1
#define False 0
#define Bool int
#define max_size 4194301
#define block_size 1024

typedef struct file { //file structure
	int start;  //start blocks number
	char name[20]; //file's name
	int editing_date[14]; //the last edit date
	Bool dirt; //whether the file has been edited
	int create_date[14]; //create date
	int protect[3]; //authorization, write/ read/ operation
	int Size; ////how many bytes do the file has
	struct file* next; //indicating the next file under the same directory
}File;

typedef struct dir { //directory structure
	char name[10]; //directory's name
	struct file* file_head; //the head of files chain under this directory
	struct dir* dir_head; //the head of directories chain belong to this directory
	struct dir* father; //father directory
	int date[14]; //create date
	struct dir* next; //indicating the next child directory under the same father
}Dir;

typedef struct open { //opened file structure
	struct file* head; //the opened file
	struct open* next; //the opened file chain
	struct dir* this_dir; //the directory where the file locate
}Open;

typedef struct disk_stack { //the blocks stack
	struct disk_stack* next; //the next structure of the stack
	int no; //the block number
}Stack;

typedef struct disk { //the disk structure
	int max_blocks; //the max blocks in the disk
	int current_use; //current using blocks
	struct disk_stack* top; //top pointer of the stack
	struct disk_stack* base; //base pointer of the stack
}Disk;

//global variables declare in main()
extern char content[max_size];  //the real disk
extern Disk* the_disk;  //the empty dick
extern Dir* the_dir;  //the base directory
extern Dir* current_dir; //the current directory
extern Open* current_open; //opened files
extern int choice;
extern char current_path[500];

//providing API for other program
Bool createFile(char name[], int protect[]); //create a new file
Bool deleteFile(char name[]); //delete a file
Bool openFile(char name[]); //open a file
Bool closeFile(char name[]); //close a file
Bool closeFile_auto(char name[]); //close a file
char* readFile(char name[]); //read the contents of a file
char* readFile_auto(char name[]); //read the contents of a file
Bool writeFile(char name[], char add[]); //write new materials
Bool writeFile_auto(char name[], char add[]); //write new materials
Bool createDir(char name[]); //create a new directory
Bool deleteDir(char name[]); //delete a directory
void initialize();

//API for user
Bool cd(char name[]); //enter a child directory
Bool last(Dir* currentDir); //return to the father directory
Bool show(Dir* currentDir); //the directories and the files under this directory
void get_date(int Time[]); //get the current year/ month/ day

						   //inner functions

						   //main() will call:
Disk* initializeDisk_1(); //initialize the disk
Disk* initializeDisk_2();
Dir* initializeDir(); //initialize the directory (the base directory)
Open* initializeOpen(); //initialize the opened files list
void showDir(Dir* currentDir); //show the path of current directory
void free_all(Disk* theDisk, Dir* theDir, Dir* currentDir, Open* currentOpen);
void free_dir(Dir* theDir);
//plus all the API above

//createFile() will call:
int find_empty(Disk* theDisk); //find an empty blocks
File* initializeFile(char name[], int no, int protect[]); //initialize file node
Bool add_file(File* the_file, Dir* currentDir); //add the file into directory

												//deleteFile() will call:
int remove_file(char name[], Dir* currentDir, Open* currentOpen); //remove the file from the current directory and return the start block of that file
Bool clear_block_1(int block, Disk* theDisk); //clear the blocks and add them into the stack
Bool clear_block_2(int block, Disk* theDisk);
Bool is_open(char name[], Dir* currentDir, Open* currentOpen);

//openFile() will call:
File* find_file(char name[], Dir* currentDir); //find the file under this directory
Bool open_file(File* this_file, Open* currentOpen, Dir* currentDir); //add the file into the opened files list

																	 //closeFile() will call:
Bool close_file(char name[], Open* currentOpen); //delete the file from the opened files list
Bool close_file_auto(char name[], Open* currentOpen); //delete the file from the opened files list

													  //close_file will call:
File* find_opened_file(char name[], Open* currentOpen); //find the file in the opened files list, extinguish with the file with the same name
File* find_opened_file_auto(char name[], Open* currentOpen); //find the file in the opened files list, extinguish with the file with the same name

															 //readFile() will call:
															 //File* find_opened_file(char name[], Open* currentOpen); //find the file in the opened files list, extinguish with the file with the same name
char* read_material_1(int start_block, Disk* theDisk, int Size); //read the materials from the disk
char* read_material_2(int start_block, Disk* theDisk, int Size);

//writeFile() will call:
//File* find_opened_file(char name[], Open* currentOpen); //find the file in the opened files list
Bool write_material_1(char* new_material, int start_block, Disk* theDisk, int Size, File* this_file); //add the new material into the disk, and update the file's size
Bool write_material_2(char* new_material, int start_block, Disk* theDisk, int Size, File* this_file);

//write_material() will call:
int if_full_1(int length, int current_block, Disk* theDisk); //judge whether the current block can hole the current length, if can return -1, else return the additional number of blocks
int if_full_2(int length, int current_block, Disk* theDisk);

//createDir() will call:
Bool create_dir(char name[], Dir* currentDir); //add the new directory into the list

											   //deleteDir() will call:
Bool delete_dir(char name[], Dir* currentDir); //delete the directory and the files, directories under this directory
Bool find_same_file(char name[], Dir* currentDir);
Bool find_same_dir(char name[], Dir* currentDir);
void present();



#endif // FS_HEAD_H_INCLUDED
