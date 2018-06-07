#include "fs_head.h"

char content[max_size];  //the real disk
Disk* the_disk;  //the empty dick
Dir* the_dir;  //the base directory
Dir* current_dir; //the current directory
Open* current_open; //opened files's empty head
int choice;
char current_path[500];

void initialize()
{
    FILE *fp;
    fp=fopen("config.txt","r");
    while(!feof(fp))
        fscanf(fp,"%d",&choice);
    fclose(fp);

    the_dir = initializeDir();
    if(choice == 1)
    {
        the_disk = initializeDisk_1();
    }
    else
    {
        the_disk = initializeDisk_1();
    }
    current_dir = the_dir;
    current_open = initializeOpen();
}

Bool add_file(File* the_file, Dir* currentDir)
{
    File* head = currentDir->file_head;
    if(currentDir->file_head == NULL)
    {
        currentDir->file_head = the_file;
    }
    else
    {
        for(;head->next != NULL; head = head->next);
        head->next = the_file;
    }
    return True;
}

Bool cd(char name[])
{
    Dir* child = current_dir->dir_head;
    while(child != NULL)
    {
        if(strcmp(name, child->name) == 0)
        {
            current_dir = child;
            strcat(current_path, "\\");
            strcat(current_path, name);
            return True;
        }
        else
        {
            child = child->next;
        }
    }
    return False;
}

Bool clear_block_1(int block, Disk* theDisk)
{
    int temp=block;
    int i=temp*block_size;  //find the block in the dick
    int j=0;
    for(j=0;j<(block_size-1);j++)
    {
        content[i+j]='\0';  //clear the material
    }
    theDisk->current_use--;
    Stack* new_s=(Stack*)malloc(sizeof(Stack));
    new_s->next=NULL;
    new_s->no=temp;
    theDisk->top->next=new_s;
    theDisk->top=new_s;
    if(content[i+j]!=-1)
    {
        int mid=content[i+j];
        content[i+j]=-1;
        if(clear_block_1(mid,theDisk)!=1)
            return 0;
    }
    return 1;
}

Bool clear_block_2(int block, Disk* theDisk)
{
    int temp=block;
    char i=content[temp*block_size];  //find the block in the disk
    int k=0,j=0;
    for(k=0;k<block_size&&i!='\0';k++)
    {
        i=content[temp*block_size+k];
        for(j=0;j<block_size;j++)
        {
            content[(int)i+j]='\0';  //clear the material
        }
        theDisk->current_use--;
        Stack* new_s=(Stack*)malloc(sizeof(Stack));
        new_s->next=NULL;
        new_s->no=(int)i;
        theDisk->top->next=new_s;
        theDisk->top=new_s;

    }
    for(j=0;j<block_size;j++)
        content[temp*block_size+j]='\0';

    theDisk->current_use--;
    Stack* new_s=(Stack*)malloc(sizeof(Stack));
    new_s->next=NULL;
    new_s->no=temp;
    theDisk->top->next=new_s;
    theDisk->top=new_s;
    return 1;
}

Bool close_file(char name[], Open* currentOpen)
{
    File* the_file = find_opened_file(name, currentOpen);
    Open* head = currentOpen->next;
    Open* last = NULL;
    if(the_file != NULL)
    {
        for(; head != NULL; head = head->next)
        {
            if(head->head == the_file)
            {
                if(head == currentOpen->next)
                {
                    currentOpen->next = head->next;
                }
                else
                {
                    last->next = head->next;
                }
                free(head);
                return True;
            }
            last = head;
        }
    }
    return False;
}

Bool close_file_auto(char name[], Open* currentOpen)
{
    File* the_file = find_opened_file_auto(name, currentOpen);
    Open* head = currentOpen->next;
    Open* last = NULL;
    if(the_file != NULL)
    {
        for(; head != NULL; head = head->next)
        {
            if(head->head == the_file)
            {
                if(head == currentOpen->next)
                {
                    currentOpen->next = head->next;
                }
                else
                {
                    last->next = head->next;
                }
                free(head);
                return True;
            }
            last = head;
        }
    }
    return False;
}

Bool closeFile(char name[])
{
    return close_file(name, current_open);
}

Bool closeFile_auto(char name[])
{
    return close_file_auto(name, current_open);
}

Bool create_dir(char name[], Dir* currentDir)
{
    Dir* new_one = (Dir*)malloc(sizeof(Dir));
    Dir* head = currentDir->dir_head;
    strcpy(new_one->name,name);
    new_one->file_head = NULL;
    new_one->dir_head = NULL;
    new_one->father = NULL;
    get_date(new_one->date);
    new_one->next = NULL;
    if(currentDir->dir_head == NULL)
    {
        currentDir->dir_head = new_one;
        new_one->father = currentDir;
    }
    else
    {
        for(;head->next != NULL; head = head->next);
        new_one->father = currentDir;
        head->next = new_one;
    }
    return True;
}


Bool createDir(char name[])
{
    if(!find_same_dir(name, current_dir))
    {
        char path[500];
        strcpy(path, current_path);
        strcat(path, "\\");
        strcat(path, name);
		//printf("%s\n", path);
        //CreateDirectory((LPCWSTR)path, NULL);
		mkdir(path);
		return create_dir(name, current_dir);
    }
    else
    {
        printf("There is a directory with the same name under this directory.\n");
        return False;
    }
}

Bool find_same_dir(char name[], Dir* currentDir)
{
    Dir* head = currentDir->dir_head;
    for(;head != NULL; head = head->next)
    {
        if(strcmp(head->name, name) == 0)
        {
            return True;
        }
    }
    return False;
}


Bool createFile(char name[], int protect[])
{
    /*int block_no = find_empty(the_disk);
    //printf("%d\n",block_no);
    File* new_file = initializeFile(name, block_no, protect);
    return add_file(new_file, current_dir);*/
    if(!find_same_file(name, current_dir))
    {
        int block_no = find_empty(the_disk);
		//printf("5: %d", content[block_no * block_size + block_size - 1]);
		if (block_no != -2)
		{
			char temp[500];
			strcpy(temp, current_path);
			strcat(temp, "\\");
			strcat(temp, name);
			strcat(temp, ".txt");
			File* new_file = initializeFile(name, block_no, protect);
			//printf("%s\n", temp);
			FILE* f = fopen(temp, "w+");
			fclose(f);
			return add_file(new_file, current_dir);
		}
		else
		{
			printf("There is not enough space!\n");
		}
    }
    else
    {
        printf("There is a file with the same name under this directory.\n");
        return False;
    }
}

Bool back()
{
	return last(current_dir);
}

Bool delete_dir(char name[], Dir* currentDir)
{
    Dir* head = currentDir->dir_head;
    Dir* last = NULL;
    if(currentDir -> dir_head == NULL)
    {
        return False;
    }
    else
    {
        for(;head != NULL; head = head->next)
        {
            if(strcmp(head->name, name) == 0)
            {
                char path[500];
                strcpy(path, current_path);
                strcat(path, "\\");
                strcat(path, name);
                rmdir(path);
                if(head == currentDir->dir_head)
                {
                    currentDir->dir_head = head->next;
                }
                else
                {
                    last->next = head->next;
                }
                File* head_file = head->file_head;
                File* next = NULL;
                while(head_file != NULL)
                {
                    next = head_file->next;
                    if(choice==1)
                        clear_block_1(head_file->start, the_disk);
                    else
                        clear_block_2(head_file->start, the_disk);
                    free(head_file);
                    head_file = next;
                }
                free(head);
                return True;
            }
            last = head;
        }
    }
    return False;
}

Bool deleteDir(char name[])
{
    return delete_dir(name, current_dir);
}



Bool deleteFile(char name[])
{
    int block_no = remove_file(name, current_dir, current_open);
    //printf("%d",block_no);
    if(block_no != -1)
    {
        if(choice==1)
            return clear_block_1(block_no, the_disk);
        else
            return clear_block_2(block_no, the_disk);
    }
    else
    {
        return False;
    }
}



int find_empty(Disk* the_disk)
{
    int re=the_disk->top->no;
    if(re!=-2)
    {
        (the_disk->current_use)++;
        Stack* mid=the_disk->base;
        while(mid->next!=the_disk->top)
        {
            mid=mid->next;
        }
        free(the_disk->top);
        mid->next=NULL;
        the_disk->top=mid;
    }
    return re;
}



File* find_file(char name[],Dir* currentDir)
{
    File* head = current_dir->file_head;
    for(;head != NULL; head = head->next)
    {
        if(strcmp(head->name, name) == 0)
        {
            return head;
        }
    }
    return head;
}

File* find_opened_file(char name[], Open* currentOpen)
{
    char choice;
    Open* headOpen = currentOpen->next;
    for(;headOpen != NULL; headOpen = headOpen->next)
    {
        if(strcmp(headOpen->head->name, name) == 0)
        {
            printf("Is your file under this directory:");
            showDir(headOpen->this_dir);
            printf("\t (Y/n):");
            scanf("%c", &choice);
            getchar();
            if(choice == 'Y')
                return headOpen->head;
        }
    }
    return NULL;
}

File* find_opened_file_auto(char name[], Open* currentOpen)
{
    char choice;
    Open* headOpen = currentOpen->next;
    for(;headOpen != NULL; headOpen = headOpen->next)
    {
        if(strcmp(headOpen->head->name, name) == 0)
        {
            return headOpen->head;
        }
    }
    return NULL;
}



void free_all(Disk *theDisk, Dir* theDir, Dir* currentDir, Open* currentOpen)
{
    Stack* disk_temp = theDisk->base;
    Stack* disk_next = NULL;
    while(disk_temp != theDisk->top)
    {
        disk_next = disk_temp->next;
        free(disk_temp);
        disk_temp = disk_next;
    }
    free(disk_temp);
    free(theDisk);

    free_dir(theDir);

    Open* open_temp = currentOpen;
    Open* open_next = NULL;
    while(open_temp != NULL)
    {
        open_next = open_temp->next;
        free(open_temp);
        open_temp = open_next;
    }
}


void free_dir(Dir* theDir)
{
    File* file_temp = theDir->file_head;
    File* file_next = NULL;
    while(file_temp != NULL)
    {
        file_next = file_temp->next;
        free(file_temp);
        file_temp = file_next;
    }
    if(theDir->next != NULL)
    {
        free_dir(theDir->next);
    }
    if(theDir->dir_head != NULL)
    {
        free_dir(theDir->dir_head);
    }
    free(theDir);
}


void get_date(int Time[])
{//get the current year, month, day, hour, minute and second
    struct tm* lt;
    time_t t;
    time(&t);
    lt = localtime(&t);
    Time[0] = (lt->tm_year+1900)/1000;
    Time[1] = ((lt->tm_year+1900)-Time[0]*1000)/100;
    Time[2] = ((lt->tm_year+1900)-Time[0]*1000-Time[1]*100)/10;
    Time[3] = (lt->tm_year+1900)%10;
    Time[4] = (lt->tm_mon)/10;
    Time[5] = (lt->tm_mon)%10;
    Time[6] = (lt->tm_mday)/10;
    Time[7] = (lt->tm_mday)%10;
    Time[8] = (lt->tm_hour)/10;
    Time[9] = (lt->tm_hour)%10;
    Time[10] = (lt->tm_min)/10;
    Time[11] = (lt->tm_min)%10;
    Time[12] = (lt->tm_sec)/10;
    Time[13] = (lt->tm_sec)%10;
}


Dir* initializeDir()
{//initialize the base directory
    Dir* new_one = (Dir*)malloc(sizeof(Dir));
    new_one->father = NULL;
    new_one->dir_head = NULL;
    new_one->file_head = NULL;
    get_date(new_one->date);
    new_one->next = NULL;
    strcpy(new_one->name, "User");
    return new_one;
}



Disk* initializeDisk_1()
{//initialize the disk structure and the stack structure
    getcwd(current_path, 500);
	strcat(current_path, "\\");
	strcat(current_path, "system");

    int i=0;
    while((i+1)*block_size<=max_size)
    {
        content[i*block_size+block_size-1]=-1;
        i++;
    }
    Disk* new_one=(Disk*)malloc(sizeof(Disk));
    new_one->current_use=0;
    new_one->max_blocks=max_size/block_size;
    new_one->base=(Stack*)malloc(sizeof(Stack));
    new_one->base->no=-2;
    new_one->base->next=NULL;
    new_one->top=new_one->base;
    for(i=0;i<(new_one->max_blocks);i++)
    {
        Stack* n_one=(Stack*)malloc(sizeof(Stack));
        new_one->top->next=n_one;
        n_one->no=i;
        n_one->next=NULL;
        new_one->top=n_one;
    }
    //printf("%d\n",new_one->top->no);
    return new_one;
}

Disk* initializeDisk_2()
{//initialize the disk structure and the stack structure
	getcwd(current_path, 500);
	strcat(current_path, "\\");
	strcat(current_path, "system");

    int i;
    Disk* new_one=(Disk*)malloc(sizeof(Disk));
    new_one->current_use=0;
    new_one->max_blocks=max_size/block_size;
    new_one->base=(Stack*)malloc(sizeof(Stack));
    new_one->base->no=-2;
    new_one->base->next=NULL;
    new_one->top=new_one->base;
    for(i=0;i<(new_one->max_blocks);i++)
    {
        Stack* n_one=(Stack*)malloc(sizeof(Stack));
        new_one->top->next=n_one;
        n_one->no=i;
        n_one->next=NULL;
        new_one->top=n_one;
    }
    //printf("%d\n",new_one->top->no);
    return new_one;
}


File* initializeFile(char name[], int no, int protect[])
{
    File* new_one = (File*)malloc(sizeof(File));
    get_date(new_one->create_date);
    get_date(new_one->editing_date);
    new_one->dirt = False;
    strcpy(new_one->name, name);
    new_one->next = NULL;
    new_one->Size = 0;
    new_one->start = no;
    new_one->protect[0] = protect[0];
    new_one->protect[1] = protect[1];
    new_one->protect[2] = protect[2];
    return new_one;
}


Open* initializeOpen()
{//initialize the opened files list
    Open* new_one = (Open*)malloc(sizeof(Open));
    new_one->head = NULL;
    new_one->next = NULL;
    new_one->this_dir = NULL;
    return new_one;
}

int if_full_1(int length, int current_block, Disk* theDisk)
{
    while(content[current_block*block_size+block_size-1]!=-1)
    {
		//printf("%d\n", content[current_block*block_size + block_size - 1]);
        current_block=(int)content[current_block*block_size+block_size-1];
    }
    int number=0;
    int j;
    for(j=0;j<(block_size-1);j++)
    {
        if(content[current_block*block_size+j]=='\0')
            number++;
    }
    if(number>=length)//这种情况是块大小够，-2是没有空闲块了便于区分
        return -1;
    else
        return find_empty(theDisk);
}

int if_full_2(int length, int current_block, Disk* theDisk)
{
    int i=0;
    while(content[current_block*block_size+i+1]!='\0')
    {
        i++;
    }
    if(content[current_block*block_size+i]=='\0')
        return find_empty(theDisk);
    else
    {
        current_block=(int)content[current_block*block_size+i];
        int number=0;
        int j;
        for(j=0;j<block_size;j++)
        {
            if(content[current_block*block_size+j]=='\0')
                number++;
        }
        if(number>=length)//这种情况是块大小够，-2是没有空闲块了便于区分
            return -1;
        else
            return find_empty(theDisk);
    }
}

Bool is_open(char name[], Dir* currentDir, Open* currentOpen)
{
    Open* head = currentOpen->next;
    for(;head != NULL; head = head->next)
    {
        if(strcmp(head->head->name, name) == 0)
        {
            if(head->this_dir == currentDir)
            {
                return True;
            }
        }
    }
    return False;
}

Bool last(Dir* currentDir)
{
	//printf("%s\n", current_path);
    if(currentDir->father != NULL)
    {
        current_dir = currentDir->father;
		int i = strlen(current_path);
		while (i >= 0)
		{
			if (current_path[i] == '\\')
			{
				current_path[i] = '\0';
				break;
			}
			i--;
		}
		//printf("%s\n", current_path);
        return True;
    }
    return False;
}


Bool open_file(File* this_file, Open* currentOpen, Dir* currentDir)
{
    Open* new_open = (Open*)malloc(sizeof(Open));
    new_open->head = this_file;
    new_open->next = NULL;
    new_open->this_dir = currentDir;
    Open* headOpen = currentOpen;
    for(; headOpen->next != NULL; headOpen = headOpen->next);
    headOpen->next = new_open;
    return True;
}

Bool openFile(char name[])
{
    File* this_file = find_file(name, current_dir);
    if(this_file != NULL)
    {
        if(this_file->protect[2] == 0)
        {
            printf("You have no authority to open this file.\n");
            return False;
        }
        return open_file(this_file, current_open, current_dir);
    }
    else
    {
        return False;
    }
}


char* read_material_1(int start_block, Disk* theDisk, int Size)
{
    //printf("%d ok\n",start_block);
	char *str;
	str = (char*)malloc(Size + 1);
    int x=start_block*block_size;  //find the block in the disk
    int i,j=0;
    for(i=0;content[x+i]!='\0'&&i<(block_size-1);i++)
    {
        str[j]=content[x+i];  //read into the string array
        j++;
    }
    while(content[x+block_size-1]!=-1)
    {
        x=content[x+block_size-1]*block_size;
        for(i=0;content[x+i]!='\0'&&i<(block_size-1);i++)
        {
            str[j]=content[x+i];
            j++;
        }
    }
    str[j]='\0';
    return str;
}


char* read_material_2(int start_block, Disk* theDisk, int Size)
{
    char *str = (char*)malloc(Size+1);
    int x=start_block*block_size;  //find the block in the disk
    int k=0;
    char y=content[x+k];
    //printf("%c\n",y);
    int i,j=0;
    while(y!='\0')
    {
        for(i=0;content[(int)y*block_size+i]!='\0'&&i<block_size;i++)
        {
            str[j]=content[(int)y*block_size+i];  //read into the string array
            j++;
        }
        k++;
        y=content[x+k];
    }
    str[j]='\0';
    return str;
}


char* readFile(char name[])
{
    File* this_file = find_opened_file(name, current_open);
    if(this_file != NULL)
    {
        if(this_file->protect[1] == 0)
        {
            printf("You have no right to read this file!\n");
            return False;
        }
        if(choice==1)
            return read_material_1(this_file->start, the_disk, this_file->Size);
        else
            return read_material_2(this_file->start, the_disk, this_file->Size);
    }
    return False;
}

char* readFile_auto(char name[])
{
    File* this_file = find_opened_file_auto(name, current_open);
    if(this_file != NULL)
    {
        if(this_file->protect[1] == 0)
        {
            printf("You have no right to read this file!\n");
            return False;
        }
        if(choice==1)
            return read_material_1(this_file->start, the_disk, this_file->Size);
        else
            return read_material_2(this_file->start, the_disk, this_file->Size);
    }
    return False;
}


int remove_file(char name[], Dir* currentDir, Open* currentOpen)
{
    File* head;
    File* last = NULL;
    if(is_open(name, currentDir, currentOpen))
    {
        return -1;
    }
    for(head = currentDir->file_head; head != NULL; head = head->next)
    {
        if(strcmp(head->name, name) == 0)
        {
            char path[500];
            strcpy(path, current_path);
            strcat(path, "\\");
            strcat(path, name);
            strcat(path, ".txt");
            remove(path);
            if(head == currentDir->file_head)
            {
                currentDir->file_head = head->next;
            }
            else
            {
                last->next = head->next;
            }
            int block_no = head->start;
            free(head);
            return block_no;
        }
        last = head;
    }
    return -1;
}

Bool show(Dir* currentDir)
{
    Dir* headDir = currentDir->dir_head;
    File* headFile = currentDir->file_head;
    if(headDir != NULL)
    {
        printf("The directories under current directory:\n");
        for(;headDir != NULL; headDir = headDir->next)
            printf("\t\t%s\n",headDir->name);
    }
    else
    {
        printf("There is no directory under current directory.\n");
    }
    if(headFile != NULL)
    {
        printf("\nThe files under current directory:\n");
        for(;headFile != NULL; headFile = headFile->next)
            printf("\t\t%s\n",headFile->name);
    }
    else
    {
        printf("There is no file under current directory.\n");
    }
    return True;
}



void showDir(Dir* currentDir)
{
    if(currentDir->father != NULL)
        showDir(currentDir->father);
    printf("\\%s",currentDir->name);
}

Bool write_material_1(char* new_material, int s_block, Disk* theDisk, int Size, File* this_file)
{
    int start_block=s_block;
	//printf("1:%d\n", start_block);
    int tag=if_full_1(Size,start_block,theDisk);
    if(tag==-1) //if the left space is enough
    {
        int i=0;
        int j=0;
		while (content[start_block*block_size + block_size - 1] != -1)
		{
			//printf("4: %d", content[start_block*block_size + block_size - 1]);
			start_block=(int)content[start_block*block_size+block_size-1];
			//printf("3: %d", start_block);
		}
            

		//printf("2:%d\n", start_block);
        while(Size>0)
        {
            if(content[start_block*block_size+i]=='\0')
            {
				//printf("%d %d %d\n", start_block, block_size, i);
                content[start_block*block_size+i]=new_material[j];
                Size--;
                j++;
            }

            i++;
        }
        this_file->Size+=strlen(new_material);
        return 1;
    }
    else //left space if not enough
    {
        int j=0;
        int i=0;
        if(tag==-2)
        {
            printf("There is not enough space!\n");
            return 0;
        }
        else
        {
            while(content[start_block*block_size+block_size-1]!=-1)
                start_block=(int)content[start_block*block_size+block_size-1];
            while(j<(block_size-1))
            {
                if(content[start_block*block_size+j]=='\0')
                {
                    content[start_block*block_size+j]=new_material[i];
                    Size--;
                    i++;
                }
                j++;
            }
            content[start_block*block_size+block_size-1]=(char)tag;
            //printf("%s ok\n",new_material);
            char* mid_material=&new_material[i];
            //printf("%s ok\n",mid_material);
            if(write_material_1(mid_material,s_block,theDisk,Size, this_file)!=1)
                return 0;
            this_file->Size+=strlen(new_material);
            return 1;
        }
    }
}


Bool write_material_2(char* new_material, int s_block, Disk* theDisk, int Size, File* this_file)
{
    int start_block=s_block;
    int tag=if_full_2(Size,start_block,theDisk);
    //printf("%d\n",tag);
    if(tag==-1) //if the left space is enough
    {
        int i=0;
        int j=0;
        int k=0;
        while(content[start_block*block_size+k+1]!='\0')
            k++;
        int m=(int)content[start_block*block_size+k];
        while(Size>0)
        {
            if(content[m*block_size+i]=='\0')
            {
                content[m*block_size+i]=new_material[j];
                Size--;
                j++;
            }
            i++;
        }
        this_file->Size+=strlen(new_material);
        return 1;
    }
    else //left space if not enough
    {
        int j=0;
        int i=0;
        if(tag==-2)
        {
            printf("There is not enough space!\n");
            return 0;
        }
        else
        {
            int k=0;
            while(content[start_block*block_size+k+1]!='\0')
                k++;
            if(content[start_block*block_size+k]=='\0')
            {
                content[start_block*block_size+k]=(char)tag;
                if(write_material_2(new_material,s_block,theDisk,Size, this_file)!=1)
                    return 0;
                else
                    return 1;
            }
            else
            {
                int m=(int)content[start_block*block_size+k];
                while(j<block_size)
                {
                    if(content[m*block_size+j]=='\0')
                    {
                        content[m*block_size+j]=new_material[i];
                        Size--;
                        i++;
                    }
                    j++;
                }
                content[start_block*block_size+k+1]=(char)tag;
                char* mid_material=&new_material[i];
                if(write_material_2(mid_material,s_block,theDisk,Size, this_file)!=1)
                    return 0;
            }
            this_file->Size+=strlen(new_material);
            return 1;
        }
    }
}


Bool writeFile(char name[], char add[])
{
    File* this_file = find_opened_file(name, current_open);
    if(this_file != NULL)
    {
        if(this_file->protect[0] == 0)
        {
            printf("You have no right to write this file!\n");
            return False;
        }
        //-----
        char temp[500];
        strcpy(temp, current_path);
        strcat(temp, "\\");
        strcat(temp, name);
        strcat(temp, ".txt");
        FILE *f = fopen(temp, "a");
        fprintf(f, add);
        fclose(f);
        //-------
        if(choice==1)
            return write_material_1(add, this_file->start, the_disk, strlen(add), this_file);
        else
            return write_material_2(add, this_file->start, the_disk, strlen(add), this_file);
    }
    return False;
}

Bool writeFile_auto(char name[], char add[])
{
    File* this_file = find_opened_file_auto(name, current_open);
    if(this_file != NULL)
    {
        if(this_file->protect[0] == 0)
        {
            printf("You have no right to write this file!\n");
            return False;
        }
		char temp[500];
		strcpy(temp, current_path);
		strcat(temp, "\\");
		strcat(temp, name);
		strcat(temp, ".txt");
		FILE *f = fopen(temp, "a");
		fprintf(f, add);
		fclose(f);
        if(choice==1)
            return write_material_1(add, this_file->start, the_disk, strlen(add), this_file);
        else
            return write_material_2(add, this_file->start, the_disk, strlen(add), this_file);
    }
    return False;
}

Bool find_same_file(char name[], Dir* currentDir)
{
	File* head = currentDir->file_head;
	for (; head != NULL; head = head->next)
	{
		if (strcmp(head->name, name) == 0)
		{
			return True;
		}
	}
	return False;
}

void present()
{
    char command[500];
    while(True)
    {
        showDir(current_dir);
        printf(">");
        gets_s(command);
        if(command[2] == ' ')
        {
            if(command[0] == 'c')
            {
                if(command[1] == 'f')
                {
                    int protect[3];
                    if(command[strlen(command)-1] == '1' || command[strlen(command)-1] == '0')
                    {
						if ((command[strlen(command) - 2] == ' ') && (command[strlen(command) - 3] == '1' || command[strlen(command) - 3] == '0'))
                        {
                            int i=3;
                            while(command[i] != ' ')
                            {
                                i++;
                            }
                            command[i] = '\0';
                            protect[0] = command[i+1] - '0';
                            protect[1] = command[i+3] - '0';
                            protect[2] = command[i+5] - '0';
                        }
                    }
                    else
                    {
                        protect[0] = 1;
                        protect[1] = 1;
                        protect[2] = 1;
                    }
                    if(!createFile(command+3, protect))
                    {
                        printf("Create file failed.\n");
                    }
                }
                else if(command[1] == 'l')
                {
                    if(!closeFile(command+3))
                    {
                        printf("Close file failed. No such an opened file.\n");
                    }
                }
                else if(command[1] == 'i')
                {
                    if(!createDir(command+3))
                    {
                        printf("Create directory failed.\n");
                    }
                }
                else if(command[1] == 'd')
                {
                    if(!cd(command+3))
                    {
                        printf("Enter directory failed. No such a directory.\n");
                    }
                }
                else
                {
                    printf("There is no such an command:%s!!\n",command);
                }
            }
            else if(command[0] == 'd')
            {
                if(command[1] == 'f')
                {
                    if(!deleteFile(command+3))
                    {
                        printf("Delete file failed. No such a file or the file is opening.\n");
                    }
                }
                else if(command[1] == 'i')
                {
                    if(!deleteDir(command+3))
                    {
                        printf("Delete directory failed. No such a directory.\n");
                    }
                }
                else
                {
                    printf("There is no such an command:%s!!\n",command);
                }
            }
            else if(command[0] == 'o')
            {
                if(command[1] == 'p')
                {
                    if(!openFile(command+3))
                    {
                        printf("Open file failed. No such an opened file.\n");
                    }
                }
                else
                {
                    printf("There is no such an command:%s!!\n",command);
                }
            }
            else if(command[0] == 'r')
            {
                if(command[1] == 'e')
                {
                    char * material = readFile(command+3);
                    if(material != NULL)
                    {
                        if(strlen(material) == 0)
                        {
                            printf("Read an empty file. Or no such an opened file.\n");
                        }
                        else
                        {
                            printf("%s\n",material);
                        }
                    }
                    else
                    {
                        printf("Read an empty file. Or no such an opened file.\n");
                    }
                    free(material);
                }
                else
                {
                    printf("There is no such an command:%s!!\n",command);
                }
            }
            else if(command[0] == 'w')
            {
                if(command[1] == 'r')
                {
                    int i=3;
                    while(command[i] != ' ')
                    {
                        i++;
                    }
                    command[i] = '\0';
                    i+=1;
                    if(!writeFile(command+3, command+i))
                    {
                        printf("Write file failed. No such an opened file. Or no enough space.\n");
                    }
                }
                else
                {
                    printf("There is no such an command:%s!!\n",command);
                }
            }

            else
            {
                printf("There is no such an command:%s!!\n",command);
            }
        }
        else if(command[0] == 'l')
        {
            if(command[1] == 'a')
            {
                if(!last(current_dir))
                {
                    printf("Back to father directory failed. Current at base directory.\n");
                }
            }
            else
            {
                printf("There is no such an command:%s!!\n",command);
            }
        }
        else if(command[0] == 's')
        {
            if(command[1] == 'h')
            {
                if(!show(current_dir))
                {
                    printf("Show directory failed.\n");
                }
            }
            else
            {
                printf("There is no such an command:%s!!\n",command);
            }
        }
        else if(strcmp(command, "exit to menu") == 0)
        {
            break;
        }
        else
        {
            printf("Command format wrong:%s!!\n",command);
        }
    }
    free_all(the_disk, the_dir, current_dir, current_open);
}
//use file system

