
//HE ZHU, UT Austin, hz6627
//Last Test Date : 02/16/2020, passed

#ifndef _PATHMANAGER_H_
#define _PATHMANAGER_H_

#include "wish.h"
#include <unistd.h>
#include <stdlib.h>
#define MAXPATHN 128 //max path number
#define PATH01 "/bin"
#define PATH02 "/user/bin"
#define DEFAULTPATHN 2
//#define TESTON 1
#define BUFFSZ 1024



//@protypes
typedef struct _path_manager PathManager;
void build_path_manager(PathManager **ppm);
void destroy_path_manager(PathManager *ppm);
int update_path(PathManager *ppm, char *paths[], const int npaths);
void print_paths(PathManager *ppm) ;
void reset_path_manager(PathManager *ppm);
char* PathManager_has(PathManager *ppm, char* filename);



//@Header-Only Implementations
typedef struct _path_manager
{
    int npath;
    char *paths[MAXPATHN];

} PathManager;

void build_path_manager(PathManager **ppm)
{
    *ppm = (PathManager *)malloc(sizeof(PathManager)+1);
    if (ppm == NULL)
    {
        err_msg(STDERR_FILENO, errno, "build_path_manager() failed%\n");
        return;
    }
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

        }
    }
    free(ppm);
    ppm = NULL;

}

int update_path(PathManager *ppm, char *paths[], const int npaths)
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
    };

    ppm->npath = npaths;

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
    char *paths[2];
    paths[0] = (char*)malloc(BUFFSZ);
    paths[1] = (char*)malloc(BUFFSZ);
    strcpy(paths[0], PATH01);
    strcpy(paths[1], PATH02);
    update_path(ppm, paths, DEFAULTPATHN);
}

char* PathManager_has(PathManager *ppm, char* filename)
{
    // if(access(filename, X_OK)!=-1)
    // {
    //     return NULL;
    // }

    char *abs_path = (char*)malloc(BUFFSZ);
    bzero(abs_path,BUFFSZ);
    for(int i = 0 ; i < ppm->npath; i ++ )
    {
        strcat( abs_path, ppm->paths[i]);
        strcat( abs_path, "/");
        strcat( abs_path, filename);

        if(access(abs_path, X_OK) != -1)
        {
            return abs_path;
        }
        bzero(abs_path,BUFFSZ);
    }
    return NULL;
}

#endif