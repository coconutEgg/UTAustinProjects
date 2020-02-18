
#define _GNU_SOURCE
#include "wishh.h"
#include "Parser.h"
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
#include "Executor.h"
int main(int argc, char *argv[])
{
    //@Get the parent process ID for further use
    PPID = getpid();
    build_path_manager(&pm);
    reset_path_manager(pm);
    reinit_parser(&ps);
    Executor exe;

    if (argc == 1)
    {
        while (1)
        {
            print_paths(pm);
            reinit_parser(&ps);
            executor_init(&exe, &ps);

            char line[BUFFSZ];
            printf("wish>");
            fgets(line, sizeof(line), stdin);
            if(line[0] == '#')continue;
            char *rest = NULL;
            char *token;

            for (token = strtok_r(line, "&", &rest); token != NULL; token = strtok_r(NULL, "&", &rest))
            {
                parser_line(&ps, token, STDSEPS, STDOPRS);
                executor_rolling(&exe);
            }
        }
    }

    else
    {   
        //print_paths(pm);
        reinit_parser(&ps);
        executor_init(&exe, &ps);
        char *line = NULL;
        FILE *fp = fopen(argv[1], "r+b");
        size_t len;
        ssize_t nread;
        
        while ((nread = getline(&line, &len, fp)) != -1)
        {   
            if(line[0] == '#')continue;
            char *rest = NULL;
            char *token;
            for (token = strtok_r(line, "&", &rest); token != NULL; token = strtok_r(NULL, "&", &rest))
            {   
                parser_line(&ps, token, STDSEPS, STDOPRS);
                executor_rolling(&exe);
            }
        }
        free(line);
        fclose(fp);
    }

    exit(0);
}