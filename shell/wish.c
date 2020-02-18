
#define _GNU_SOURCE
#include "wish.h"
//utilities
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

//#define TESTON 1


int main(int argc, char *argv[])
{   
    //FD = STDOUT_FILENO;
    _pid = getpid();
    build_path_manager(&pm);
    reset_path_manager(pm);

    while (1)
    {      
        // print_paths(pm);
        if (argc == 1)
        {
            printf("wish>");
            fgets(cmd_buff, sizeof(cmd_buff), stdin);
            if(cmd_buff[0] =='\n')
            {
                continue;
            }
            int cnt = parser(SEPERATORS1, CMDMODE);
            execute(cnt);
            printf("\n");
        }
        else
        {
            FILE *fp = fopen(argv[1], "r+b");
            size_t _len;
            batch_buff = (char *)malloc(BUFFSZ);
            while (getline(&batch_buff, &_len, fp) != -1)
            {
                int cnt = parser(SEPERATORS1, BATCHMODE);
                execute(cnt);
            }
            exit(0); //exit after batch mode
        }
    }

    exit(0);
}