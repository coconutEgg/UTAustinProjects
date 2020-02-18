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
//      (10)void process_ls(int cnt)
//          : branch from the execute()
//      (11)void process_script(int cnt)
//          : process the script

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

#define SEPERATORS1 " \n\t\r"
#define SEPERATORS2 " \n\t\r"

//#define TESTON 1

pid_t _pid;
PathManager *pm;

int found; //has direction?

void print_strs(int n, char *strs[])
{
    //printf("count = %d\n",n);
    for (int i = 0; i < n; i++)
    {
        printf("%s ", strs[i]);
    }
    printf("\n");
}

char cmd_buff[BUFFSZ] = {0};
char *batch_buff;
char *myargs[MAXARGC];
char ERRNOFILE[128];

char OUTTGT[BUFFSZ];

int status;

#define BATCHMODE 0x001
#define CMDMODE 0x010
#define ANDMODE 0x100

void switch_space(int mode)
{
    char *pbuf = (mode == BATCHMODE) ? batch_buff : cmd_buff;
    int i = 0, j = 0;
    int l1 = strlen(pbuf);
    int l2 = 2 * l1;
    char *_pbuf = (char *)malloc(l2);
    memset(_pbuf, 0, l2);

    while (i < l1 && j < l2)
    {
        if (pbuf[i] != '>' && pbuf[i] != '&')
        {
            _pbuf[j] = pbuf[i];
            j++;
            i++;
        }
        if (pbuf[i] == '>' || pbuf[i] == '&')
        {
            _pbuf[j] = ' ';
            j++;
            _pbuf[j] = pbuf[i];
            j++;
            i++;
            _pbuf[j++] = ' ';
        }
    }
    _pbuf[strlen(_pbuf)] = '\0';
    if (mode == BATCHMODE)
    {
        free(batch_buff);
        batch_buff = _pbuf;
    }
    else
    {
        strcpy(cmd_buff, _pbuf);
        free(_pbuf);
    }
}

/*@parser the input from cmd lines*/
int parser(const char *seps, int flag)
{
    switch_space(flag);
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
    return i; //return the count of args
}

void process_ls(int cnt)
{
    char buf[4];
    strcpy(buf, "ls");
    char *abs_name = PathManager_has(pm, buf);
    if (abs_name == NULL && _pid == getpid())
    {
        //printf("pid = %d\n", (int)getpid());
        write_err(BASICERRFLAG, "", "");
    }
    else if (abs_name != NULL)
    {
        pid_t pid = fork();
        //int ret;
        if (pid < 0)
        {
            exit(0);
        }
        else if (pid == 0)
        {

            if (cnt == 1)
            {
                if (myargs[1] != NULL)
                {
                    //memory should be carefully managed
                    free(myargs[1]);
                }
                myargs[1] = (char *)malloc(BUFFSZ);
                getcwd(myargs[1], BUFFSZ);
                strcpy(myargs[0], abs_name);
                execvp(myargs[0], myargs);
            }
            else if (cnt >= 2)
            {
                strcpy(myargs[0], abs_name);
                execvp(myargs[0], myargs);
            }
            exit(1);
        }
        else
        {

            while (wait(&status) != pid)
            {}
            return;
        }
    }
}

void process_script(int cnt)
{

    if (cnt == 1)
    {
        char *filename = PathManager_has(pm, myargs[0]);
        pid_t pid = fork();

        if (pid < 0)
        {
        }
        else if (pid == 0)
        {
            if (filename != NULL)
            {
                strcpy(myargs[0], filename);
                execvp(myargs[0], myargs);
            }
            else
            {
                write_err(BASICERRFLAG, "", "");
            }
        }
        else
        {
            while (wait(&status) != pid)
            {
            }
            if (WIFEXITED(status))
            {
                //printf("Error happened");
                return;
            }
        }
    }
}

void process_path(int _cnt)
{
    //printf("process_path");
    if (_cnt == 1)
    {
        destroy_path_manager(pm);
        build_path_manager(&pm);
    }
    else
    {
        int cnt = 1 + _cnt;
        char *paths[cnt];
        paths[0] = (char *)malloc(BUFFSZ);
        paths[1] = (char *)malloc(BUFFSZ);
        strcpy(paths[0], PATH01);
        strcpy(paths[1], PATH02);
        for (int i = 2; i < cnt; i++)
        {
            DIR *dir = opendir(myargs[i - 1]);
            if (dir == NULL)
            {
                //write_err(BASICERRFLAG, "", "");
            }
            else
            {
                paths[i] = (char *)malloc(BUFFSZ);
                char buff[BUFFSZ];
                getcwd(buff, BUFFSZ);
                if (chdir(myargs[i - 1]) == 0)
                {

                    char temp[BUFFSZ];
                    getcwd(temp, BUFFSZ);
                    strcpy(paths[i], temp);
                    chdir(buff);
                }
                else
                {
                    write_err(BASICERRFLAG, "", "");
                }
            }
        }
        update_path(pm, paths, cnt);
        for (int i = 0; i < cnt; i++)
            free(paths[i]);
    }

}

int set_redirection(int narg, int *found)
{
    int index = 0;
    //int ret = 1;
    int cnt = 0;
    //if wrong with direction
    int i = 0;
    for (i = 0; i < narg; i++)
    {
        int diff = strcmp(myargs[i], ">");

        if (diff == 0)
        {
            index = i;
            *found = 1;
            cnt++;
        }

        //> cannot be at the tail
        if (diff == 0 && i == narg - 1)
        {
            return 0;
        }
        //> cannot be at the beginning
        else if (diff == 0 && i == 0)
        {
            return 0;
        }
        else if (diff == 0 && i + 2 < narg)
        {
            if (strcmp(myargs[i + 1], ">") != 0 && strcmp(myargs[i + 2], ">") != 0)
                return 0;
        }
        else if (diff == 0)
        {
        }
    }
    //get the final target output file
    strcpy(OUTTGT, myargs[i - 1]);
    //if direction is correct or no direction
    if (cnt >= 2)
    {
        return 0;
    }

    return index;
}

void execute(int narg)
{   
    found = 0;
    int index_redirect = set_redirection(narg, &found);
    if (index_redirect == 0 && found)
    {
        write_err(BASICERRFLAG, "", "");
        return;
    }
    //printf("target file %s \n", OUTTGT);
    narg = (index_redirect > 0 ? index_redirect : narg);

    if (strcmp(myargs[0], "exit") == 0)
    {
        if (narg == 1)
        {
            exit(0);
        }
        else
        {
            write_err(BASICERRFLAG, "", "");
        }
    }
    else if (strcmp(myargs[0], "cd") == 0)
    {
        if (narg != 2)
        {
            write_err(BASICERRFLAG, "", "");
        }
        else
        {
            if (chdir(myargs[1]) == -1)
            {
                write_err(BASICERRFLAG, "", "");
            };
        }
    }
    else if (strcmp(myargs[0], "ls") == 0)
    {
        if (narg == 1)
        {
        }
        else if (narg > 1)
        {
            DIR *dir = opendir(myargs[1]);
            if (dir == NULL)
            {
                write_err(ACCESSERRFLAG, "ls", myargs[1]);
                return;
            }
        }
        process_ls(narg);
    }
    else if (strcmp(myargs[0], "path") == 0)
    {
        process_path(narg);
    }
    else if (strcmp(myargs[0], "cat") == 0)
    {
    }
    else
    {
        process_script(narg);
    }

    for(int i = 0 ; i < narg; i ++ )
    {

    }
}

#endif


