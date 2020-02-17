#ifndef ERRPROCESSER_H_
#define ERRPROCESSER_H_

#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define ERRMSGSZ 128


#define BASICERR    "An error has occurred\n"
#define ACCESSERR   "%s: cannot access '%s': No such file or directory\n"

#define BASICERRFLAG 0
#define ACCESSERRFLAG 1

void err_msg(int fd,int _errno, const char* info)
{
    char errstr[ERRMSGSZ]={0};
    sprintf(errstr, "err: %s --- %s\n", strerror(_errno), info);
    write(fd, errstr, sizeof(errstr));
    return;
}

void write_err(int flag, const char *arg1, const char*arg2)
{
    if(flag == BASICERRFLAG)
    {
        write(STDERR_FILENO, BASICERR, strlen(BASICERR));
    }
    else
    {   
        char buf[128] = {0};
        sprintf(buf, ACCESSERR, arg1, arg2);
        write(STDERR_FILENO, buf, strlen(buf));
    }
}

#endif