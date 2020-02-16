
//HE ZHU, UT Austin, hz6627
//Last Test Date : 02/16/2020, passed

#ifndef _PATHMANAGER_H_
#define _PATHMANAGER_H_

#include "wish.h"

#include <unistd.h>
#include <stdlib.h>

#define MAXPATHN 128 //max path number
#define PATH01 "/bin"
#define PATH02 "/uer/bin"
#define DEFAULTPATHN 2

//#define TESTON 1

typedef struct _path_manager
{
    int npath;
    char *paths[MAXPATHN];

} PathManager;

void build_path_manager(PathManager **ppm)
{
    *ppm = (PathManager *)malloc(sizeof(PathManager));
    if (ppm == NULL)
    {
        err_msg(STDERR_FILENO, errno, "build_path_manager() failed%\n");
        return;
    }

#ifdef TESTON
    printf("path initialized");
#endif

    (*ppm)->npath = 0;
    return;
};

void destroy_path_manager(PathManager *ppm)
{
    for (int i = 0; i < ppm->npath; i++)
    {
        if (ppm->paths[i] != NULL)
        {
            free(ppm->paths[i]);

#ifdef TESTON
            printf("path %d destroyed\n", i);
#endif
        }
    }
    free(ppm);
    ppm = NULL;

#ifdef TESTON
    printf("path manager destroyed\n");
#endif
}

int update_path(PathManager *ppm, const char *paths[], const int npaths)
{
    //according to the requirement, we should overwrite the old path
    if (ppm == NULL)
    {
        err_msg(STDERR_FILENO, errno, "PathManager* not initialized");
        return 0;
    }

    for (int i = 0; i < npaths; i++)
    {
        ppm->paths[i] = (char *)malloc(strlen(paths[i]) + 1);
        if (strcpy(ppm->paths[i], paths[i]) == NULL)
        {
            err_msg(STDERR_FILENO, errno, "---update_path()");
            exit(0);
        };
#ifdef TESTON
        printf("path added\n");
#endif
        ppm->npath++;
    };

    return 1;
}

void print_paths(PathManager *ppm) //for test purpose, dup2() to other fd
{
    printf("---Total Path Number: %d ---\n", ppm->npath);
    for (int i = 0; i < ppm->npath; i++)
    {
        printf("%d: %s\n", i + 1, ppm->paths[i]);
    }
    return;
}

void reset_path_manager(PathManager *ppm) //reset the path manager to be the default
{
    const char *paths[2] = {"/bin", "/usr/bin"};
    update_path(ppm, paths, DEFAULTPATHN);
}

#endif