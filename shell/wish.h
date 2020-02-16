/*HEZHU UT Austin---- hz6627*/
/****************************/
// Structure of the project:
//
//PathManager.h:
//                 ---PART 0 : Basic Utilities---
//      (1) print_strs(print_strs(int n, char* strs[]))
//          just for print a array of strings
//
//                  ---PART 1 : PATH MANAGER ----
//
//      (2) typedef struct _path_manager
//          :manage the paths
//      (3) void build_path_manager(PathManager **ppm)
//          :initialize a PathManager object with no path members
//      (4) void destroy_path_manager(PathManager *ppm)
//          :free the memory used by PathManager object
//      (5) int update_path(PathManager* ppm, const char* paths[], const int npaths)
//          :update PathManager object's members
//      (6) void print_paths(PathManager *ppm)
//          :print the path members
//      (7) void reset_path_manager(PathManager *ppm)
//          :init the path member to be default "/bin" and "/user/bin"
//              --- PART 2 : PROCESS COMMANDS ---
//      (8) int parser(const char *seps, int flag)
//          : deal with the parser input either from cmd or batch
//      (9) void execute(int flag)
//          : deal with the execution for either batch mode or cmd mode

#ifndef WISH_H_
#define WISH_H_

#include "ErrProcesser.h"
#include "PathManager.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

//process controls
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

//Add your code here
#define BUFFSZ 1024 //default buff size
#define MAXARGC 32  //max arg count

#define ERRMSG "An error has occurred"

#define SEPERATORS1 " \n\t\r"
#define SEPERATORS2 " \n\t\r"

#define TESTON 1

void print_strs(int n, char *strs[])
{
    //printf("count = %d\n",n);
    for (int i = 0; i < n; i++)
    {
        printf("%s ", strs[i]);
    }
    printf("\n");
}

char cmd_buff[BUFFSZ];
char *batch_buff;
char *myargs[MAXARGC];
char ERRNOFILE[128];

int status;

#define BATCHMODE 0x001
#define CMDMODE 0x010
#define ANDMODE 0x100

/*@parser the input from cmd lines*/
int parser(const char *seps, int flag)
{
    char *pbuf = (flag == BATCHMODE) ? batch_buff : cmd_buff;
    int i = 0;
    char *pch = strtok(pbuf, seps);
    while (pch != NULL)
    {   
        myargs[i] = (char *)malloc(BUFFSZ);
        strcpy(myargs[i], pch);
        pch = strtok(NULL, seps);
        i++;
    }
    //printf("count = %d\n", i);
    return i; //return the count of args
}

void execute(int narg)
{  

    if(strcmp(myargs[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(myargs[0], "cd")==0)
    {   
        if(narg != 2)
        {   
            write_err(BASICERRFLAG,"","");
        }
        else
        {
            if(chdir(myargs[1])==-1)
            {
                write_err(BASICERRFLAG,"","");
            };
        }
    }
    else if( strcmp(myargs[0], "ls")==0 )
    {
        if(narg == 0)
        {
            execvp(myargs[0], myargs);
        }
        else
        {
            DIR *dir = opendir(myargs[1]);
            if(dir == NULL)
            {
                write_err(ACCESSERRFLAG,"ls",myargs[1]);
            }
        }
        
    } 

    // pid_t pid = fork();

    // if (pid < 0)
    // {
    //     exit(0);
    // }
    // else if (pid == 0)
    // {
    // }
    // else
    // {
    //     while (wait(&status) != pid)
    //     {
    //     }
    // }
}

#endif