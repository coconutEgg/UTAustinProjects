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
#include "Parser.h"

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


pid_t PPID;
PathManager *pm;
Parser ps;



#endif