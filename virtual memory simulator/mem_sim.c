#include "mem_sim.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int returnEmptyAddress(char main_memory[]);
void putIntoMainMemoty(int page, int address,int logicalmem_fd,char main_memory[]);
int ifDirty(page_descriptor[]);
void putInSwap(char[], int ,int );
int index4 = 0;

/*
This method that initializes all the parameters
*/
int init_system(page_descriptor page_table[], char main_memory[], int* logicalmem_fd , int* swapfile_fd)
{
    *logicalmem_fd = open("program.txt", O_RDONLY); //open file to read
    if(*logicalmem_fd < 0) //Check if it succeeded or failed
    {
        fprintf(stderr,"ERROR\n");
        exit(1);
    }

    *swapfile_fd = open( "swap.txt",O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP |  S_IROTH); //open file to write
    if(*swapfile_fd < 0) //Check if it succeeded or failed
    {
        fprintf(stderr,"ERROR\n");
        exit(1);
    }

    int index;
    for(index = 0; index < MEMORY_SIZE; index++) //initialize the main memory in 0
    {
        main_memory[index] = '0'; //initialize the main memory in 0
    }

    for(index = 0; index < NUM_OF_PAGES; index++) //initialize the page table memory in 0
    {
        page_table[index].valid = 0;
        page_table[index].dirty = 0;
        page_table[index].frame = -1;
    }

    for(index = 0; index < (PAGE_SIZE*NUM_OF_PAGES); index++)
    {
        lseek(*swapfile_fd, index,0);
        write(*swapfile_fd,"0",1); //write to swapfile the buffer-0
    }
}

/*
This method return the index of the empty address or -1 if all the address were fulled
*/
int returnEmptyAddress(char main_memory[])
{
    int i;
    for(i = 0; i < MEMORY_SIZE; i++) //run on all the main memory
    {
        if(main_memory[i] == '0') //if the address is empty - with '0'
            return i; //return the index of the address
    }
    return -1; //else - return -1
}

/*
This method puts into main memory
*/
void putIntoMainMemoty(int page, int address,int logicalmem_fd ,char main_memory[])
{
    char buf[PAGE_SIZE];
    lseek(logicalmem_fd, page*PAGE_SIZE, SEEK_SET); //Allows us to move within the file. The amount of characters we want to read: page*PAGE_SIZE, from the beginning of the file
    read(logicalmem_fd, buf, PAGE_SIZE); //Reading what is in the file into the buff, reads up to the page size
    int i = 0;
    while(i != PAGE_SIZE) //As long as you have not completed the page
    {
        main_memory[address+i] = buf[i]; //Insert what is in the buff into the main memory
        i++;
    }
}

/*
This method put in the swap
*/
void putInSwap(char main_memory[], int swapfile_fd, int address)
{
    char buf[PAGE_SIZE];
    int i;
    for(i = 0; i < PAGE_SIZE; i++) //Runs on page size
    {
        buf[i] = main_memory[(index4*PAGE_SIZE)+i]; //Will enter into the buff what is in the main memory
    }
    lseek(swapfile_fd, address*PAGE_SIZE, SEEK_SET); //Allows us to move within the file. The amount of characters we want to read: address*PAGE_SIZE, from the beginning of the file
    write(swapfile_fd, buf, PAGE_SIZE); //Writing the page that is in the buff into the swap
}

/*
This method check if is dirty or not (has undergone changes or not)
*/
int ifDirty(page_descriptor page_table[])
{
    int i;
    for(i = 0; i < NUM_OF_PAGES; i++) //Will run on multiple pages
    {
        if(page_table[i].frame == (index4*PAGE_SIZE))
            return i; //return the index
    }
    return -1; //else, return -1
}

/*
This function aims to ensure that the page corresponding to the address does indeed sit in the main memory and then the physical address in the memory must be accessed and the character returned
Sitting at this address
*/
char load (int address, page_descriptor page_table[], char main_memory[] , int logicalmem_fd, int swapfile_fd) {
    if (address < 0 || address >= PAGE_SIZE * NUM_OF_PAGES) //check if the address is invalid
    {
        printf("address invalid\n");
        return '\0';
    }

    int page, offset, curFrame;
    page = address / PAGE_SIZE; //The page where the character is located
    offset = address % PAGE_SIZE; //The offset where the page
    if (page_table[page].valid == 1) //If this page is already in the page table
    {
        curFrame = page_table[page].frame; //According to the frame in the table we will initialize curFrame
        return main_memory[curFrame+offset]; //From this we will know how to return the character, calculated according to the page and the oofset
    }
    else //The page is not in the main memory
    {
        int indexEmpty = returnEmptyAddress(main_memory); //Checking an index that is empty in the main memory
        if (indexEmpty > -1)  //If the index is greater than -1
        {
            putIntoMainMemoty(page, indexEmpty, logicalmem_fd, main_memory); //Sent to the method you put in the main memory
            page_table[page].valid = 1; //Change the page table to valid
            page_table[page].frame = indexEmpty;  //The frame will be according to the blank index we entered
            return main_memory[indexEmpty+offset]; //Then according to the page and the offset we can go straight back to the character
        }
        else //If the main memory is full
        {
            int i = ifDirty(page_table); //Check whether changes have been made or not
            if (page_table[i].dirty == 1) //If there were any changes
            {
                putInSwap(main_memory, swapfile_fd, i); //We put in a swap
                int x = index4*PAGE_SIZE;
                putIntoMainMemoty(page, x , logicalmem_fd, main_memory); //We put in the main memory
                page_table[page].valid = 1; //Will be valid in the position of the page
                page_table[page].frame = x; //The frame will be according to X
                page_table[i].valid = 0; //Instead we will be back to zero
                page_table[i].dirty = 0; //Instead we will be back to zero
                page_table[i].frame = -1; //Instead we will be back to zero
                if(index4 == 5)
                {
                    index4 = 0;
                }
                else
                {
                    index4++;
                }
                return main_memory[x+offset]; //Will return the character
            }
            else //If there were no changes
            {
                int x = index4*PAGE_SIZE;
                putIntoMainMemoty(page, x , logicalmem_fd, main_memory); //We put in the main memory
                page_table[page].valid = 1; //Will be valid in the position of the page
                page_table[page].frame = x; //The frame will be according to X
                page_table[i].valid = 0; //Instead we will be back to zero
                page_table[i].dirty = 0; //Instead we will be back to zero
                page_table[i].frame = -1; //Instead we will be back to zero
                if(index4==5)
                {
                    index4 = 0;
                }
                else
                {
                    index4++;
                }
                return main_memory[x+offset]; //Will return the character
            }
        }
    }
}

/*
This function aims to make sure that the page corresponding to the address does sit in the main memory and then access the physical address and store the value character in the address
this
*/
void store(int address, char val, page_descriptor page_table[], char main_memory[] , int logicalmem_fd, int swapfile_fd)
{

    if (address < 0 || address >= PAGE_SIZE * NUM_OF_PAGES) //check if the address is invalid
    {
        printf("address invalid\n");
        return ;
    }

    int page, offset, curFrame;
    page = address / PAGE_SIZE; //The page where the character is located
    offset = address % PAGE_SIZE; //The offset where the page
    if (page_table[page].valid == 1) //If this page is already in the page table
    {
        curFrame = page_table[page].frame; //According to the frame in the table we will initialize curFrame
        page_table[page].dirty = 1; //Women 1 in dirty because we made changes
        main_memory[curFrame+offset] = val; //And replace the value in the appropriate place for what we were asked to do
        return; //We're done
    }
    else //The page is not in the main memory
    {
        int indexEmpty = returnEmptyAddress(main_memory); //Checking an index that is empty in the main memory
        if (indexEmpty > -1) //If the index is greater than -1
        {
            putIntoMainMemoty(page, indexEmpty, logicalmem_fd, main_memory); //Sent to the method you put in the main memory
            page_table[page].valid = 1; //Change the page table to valid
            page_table[page].frame = indexEmpty; //The frame will be according to the blank index we entered
            page_table[page].dirty = 1; //We put 1 in Dirty because we made changes
            main_memory[indexEmpty+offset] = val; //And replace the value in the appropriate place for what we were asked to do
            return; //We're done
        }
        else //If the main memory is full
        {
            int i = ifDirty(page_table); //Check whether changes have been made or not
            if (page_table[i].dirty == 1) //If there were any changes
            {
                putInSwap(main_memory, swapfile_fd, i); //Sent to the method you put in the swap
                int x = index4*PAGE_SIZE;
                putIntoMainMemoty(page, x , logicalmem_fd, main_memory); //Sent to the method you put in the main memory
                page_table[page].valid = 1; //Change the page table to valid
                page_table[page].frame = x; //The frame will be according to X
                page_table[page].dirty = 1; //Change the page table to dirty cause we made change
                page_table[i].valid = 0; //Instead we will be back to zero
                page_table[i].dirty = 0; //Instead we will be back to zero
                page_table[i].frame = -1; //Instead we will be back to zero
                if(index4==5)
                {
                    index4 = 0;
                }
                else
                {
                    index4++;
                }
                main_memory[x+offset] = val; //Will change to val
                return; //and finish
            }
            else
            {
                int x = index4*PAGE_SIZE;
                putIntoMainMemoty(page, x , logicalmem_fd, main_memory); //Sent to the method you put in the main memory
                page_table[page].valid = 1; //Change the page table to valid
                page_table[page].frame = x; //The frame will be according to X
                page_table[page].dirty = 1; //Change the page table to dirty cause we made change
                page_table[i].valid = 0; //Instead we will be back to zero
                page_table[i].dirty = 0; //Instead we will be back to zero
                page_table[i].frame = -1; //Instead we will be back to zero
                if(index4==5)
                {
                    index4 = 0;
                }
                else
                {
                    index4++;
                }
                main_memory[x+offset] = val; //Will change to val
                return; //and finish
            }
        }
    }
}

/*
This function will print the contents of the main memory
*/
void print_memory(char main_memory[])
{
    int i;
    printf("\nPhysical memory\n");
    for(i = 0; i < MEMORY_SIZE; i++) //Run on all the main memory
    {
        if(main_memory[i] == '\0')
        {
            return;
        }
        if(i%PAGE_SIZE == 0) //After each line drop page
        {
            printf("\n");
        }
        printf("[%c] ", main_memory[i]); //Print what is in it
    }
}
