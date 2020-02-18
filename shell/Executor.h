#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include "Parser.h"
#include "stdio.h"
#include "wishh.h"
#include <unistd.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct executor_t
{
    Parser *ps;
} Executor;

void executor_rolling(Executor *exe);
void executor_run_once(char *argv[], int cnt);
void execute_ls(char *argv[], int cnt);
void execute_cd(char *argv[], int cnt);
void execute_script(char *argv[], int cnt);
void execute_path(char *argv[], int cnt);

int prepare_redirection(char *argv[], int cnt, int *index);

void executor_init(Executor *exe, Parser *_ps)
{
    exe->ps = _ps;
}

void executor_rolling(Executor *exe)
{
    if (exe->ps == NULL)
    {
        printf("Please attach parser object to executor object before rolling\n");
        return;
    }
    executor_run_once(exe->ps->args, exe->ps->argcnt);
}

void executor_run_once(char *argv[], int cnt)
{
    //
    if (argv == NULL)
    {
        perror("argv == NULL");
        exit(0);
    }

    if (cnt == 0)
    {
        return;
    }

    if (strcmp("ls", argv[0]) == 0)
    {
        // printf("run ls()\n");
        execute_ls(argv, cnt);
    }
    else if ((strcmp("cd", argv[0]) == 0))
        execute_cd(argv, cnt);
    else if ((strcmp("exit", argv[0]) == 0))
    {
        if (cnt > 1)
            write_err(BASICERRFLAG, BLANK, BLANK);
        else
            exit(0);
    }
    else if (strcmp("path", argv[0]) == 0)
    {
        execute_path(argv, cnt);
    }
    else
    {
        //printf("will run script");
        execute_script(argv, cnt);
    }
}

//wrong statement, return -1; no redirect, return 0; have redirec, return fd
int prepare_redirection(char *argv[], int cnt, int *index)
{
    int maxredindex = 0;
    //printf("---(1)---\n");
    if (strcmp(argv[0], ">") == 0 || strcmp(argv[cnt - 1], ">") == 0)
    {
        return -1;
    }
    //printf("---(2)---\n");
    int redcnt = 0;
    int i = 0;
    for (i = 0; i < cnt; i++)
    { //printf("---(3)---\n");
        if (strcmp(argv[i], ">") == 0)
        {
            maxredindex = i;
            redcnt++;
        }
        if (redcnt > 1)
        {
            return -1;
        }
    }

    //after >, no more than 1 string should be appended
    if (cnt - (maxredindex + 1) > 1)
    {
        return -1;
    }

    //printf("---(4)---\n");
    if (redcnt == 0)
    {
        return 0;
    }
    //printf("---(5)---\n");
    int fd = open(argv[cnt - 1], O_RDWR | O_CREAT, 0664);
    if (fd < 0)
    {
        //printf("---(6)---\n");
        printf("trying open file %s\n", argv[cnt - 1]);
        perror("prepare_direction()-open()");
        return -1;
    }
    //printf("---(7)---\n");
    *index = maxredindex;
    return fd;
}

void execute_path(char *argv[], int cnt)
{
    if (cnt == 1)
    { //if input is : path, then PathManager has no paths saved
        destroy_path_manager(pm);
        build_path_manager(&pm);
    }
    else if (cnt > 1)
    {
        cnt = cnt + 1;
        char *paths[cnt];
        paths[0] = (char *)malloc(BUFFSZ);
        paths[1] = (char *)malloc(BUFFSZ);
        strcpy(paths[0], PATH01);
        strcpy(paths[1], PATH02);
        for (int i = 2; i < cnt; i++)
        {
            DIR *dir = opendir(argv[i - 1]);
            if (dir == NULL)
                write_err(BASICERRFLAG, "", "");
            else
            {
                paths[i] = (char *)malloc(BUFFSZ);
                char buff[BUFFSZ];
                getcwd(buff, BUFFSZ);        //save the current address
                if (chdir(argv[i - 1]) == 0) //return value is 0, chdir successful
                {
                    char temp[BUFFSZ];
                    getcwd(temp, BUFFSZ);
                    strcpy(paths[i], temp);
                    chdir(buff);
                }
                else
                {
                    write_err(BASICERRFLAG, BLANK, BLANK);
                }
            }
        }
        update_path(pm, paths, cnt); //clear and use paths[] to update paths
    }
}

void execute_script(char *argv[], int cnt)
{
    int fd;
    //printf("In sript, cnt = %d\n", cnt);
    int maxredindex;
    int status;
    char *filename = PathManager_has(pm, argv[0]);
    //printf("filename = %s\n", filename);
    if (filename == NULL)
    {
        write_err(BASICERRFLAG, BLANK, BLANK);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
    }
    else if (pid == 0)
    {
        // if (strcmp(argv[0], "ls") == 0 || strcmp(argv[0], "cat") == 0)
        // {
            fd = prepare_redirection(argv, cnt, &maxredindex);
            //printf("script fd = %d\n",fd);
            if (fd > 0)
            {
                cnt = min(maxredindex, cnt);
                dup2(fd, STDERR_FILENO);
                dup2(fd, STDOUT_FILENO);
            }
            else if (fd < 0)
            {
                write_err(BASICERRFLAG, BLANK, BLANK);
                exit(0);
            }
        // }
        if (cnt == 1)
        {
            char *buff[2];
            strcpy(buff[0], filename);
            buff[1] = NULL;
            execv(buff[0], buff);
            exit(0);
        }
        else if (cnt == 2)
        {
            int _fd = open(argv[1], O_RDWR);
            if (fd < 0)
            {
                write_err(ACCESSERRFLAG, argv[0], argv[1]);
                return;
            }
            //char buf[BUFSZ];
            char *buff[3];
            buff[0] = (char *)malloc(BUFSZ);
            buff[1] = (char *)malloc(BUFSZ);
            strcpy(buff[0], filename);
            strcpy(buff[1], argv[cnt - 1]);
            buff[2] = NULL;
            if (execvp(buff[0], buff))
            {
                printf("execvp() err %s\n", strerror(errno));
            };
        }
        else if (cnt > 2)
        {
            printf("process script cnt > 2\n");
            char *buff[cnt + 1];
            for (int i = 0; i < cnt + 1; i + 1)
            {
                buff[i] = (char *)malloc(BUFSZ);
                if (i == 0)
                    strcpy(buff[i], filename);
                else if (i == cnt)
                    buff[i] = NULL;
                else
                {
                    strcpy(buff[i], argv[i]);
                }
            }
            if (execvp(buff[0], buff))
            {
                printf("execvp() err %s\n", strerror(errno));
            };
        }
        exit(1);
    }
    else if (pid > 0)
    {
        while (wait(&status) != pid)
        {
        }
        return;
    }
}

void execute_cd(char *argv[], int cnt)
{
    //printf("cnt=%d\n", cnt);
    if (cnt != 2)
    {
        write_err(BASICERRFLAG, BLANK, BLANK);
    }
    else
    {
        if (chdir(argv[1]) == -1)
        {
            printf("chdir err\n");
            write_err(BASICERRFLAG, BLANK, BLANK);
            return;
        }
    }
}

void execute_ls(char *argv[], int cnt)
{

    int maxredindex = 0;
    int status;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork error\n");
        exit(1);
    }
    if (pid == 0)
    {
        //printf("preaparing...\n");
        int fd = prepare_redirection(argv, cnt, &maxredindex);
        //printf("ls count: %d, maxredindex: %d\n",cnt,maxredindex);
        //printf("fd=%d\n",fd);
        if (fd > 0)
        {
            cnt = min(maxredindex, cnt);
            //printf("Successful created,cnt=%d\n",cnt);
            dup2(fd, STDERR_FILENO);
            dup2(fd, STDOUT_FILENO);
        }
        else if (fd < 0)
        {
            //printf("failed created");
            write_err(BASICERRFLAG, BLANK, BLANK);
            exit(0);
        }
        //printf("ls count %d, %s",cnt, argv[cnt-1]);
        if (cnt == 1)
        {
            char buf[] = {"ls"};
            char *abs_name = PathManager_has(pm, buf);
            if (abs_name == NULL)
            {
                write_err(BASICERRFLAG, BLANK, BLANK);
            }
            char *buff[3];
            buff[0] = (char *)malloc(BUFSZ);
            buff[1] = (char *)malloc(BUFSZ);
            strcpy(buff[0], abs_name);
            getcwd(buff[1], BUFFSZ);
            buff[2] = NULL;
            if (execvp(buff[0], buff))
            {
                printf("execvp() err %s\n", strerror(errno));
            };
        }
        else if (cnt == 2)
        {
            DIR *dir = opendir(argv[1]);
            if (dir == NULL)
            {
                write_err(ACCESSERRFLAG, "ls", argv[1]);
                return;
            }
            char buf[] = {"ls"};
            char *abs_name = PathManager_has(pm, buf);
            if (abs_name == NULL)
                write_err(BASICERRFLAG, BLANK, BLANK);
            char *buff[3];
            buff[0] = (char *)malloc(BUFSZ);
            buff[1] = (char *)malloc(BUFSZ);
            strcpy(buff[0], abs_name);
            strcpy(buff[1], argv[cnt - 1]);
            buff[2] = NULL;
            if (execvp(buff[0], buff))
            {
                printf("execvp() err %s\n", strerror(errno));
            };
        }
        exit(1);
    }
    else if (pid > 0)
    {
        while (wait(&status) != pid)
        {
        }
        return;
    }
}

#endif
