#include "wish.h"

int main()
{
    PathManager *pm;
    build_path_manager(&pm);
    //const char *strs[3] = {"/bin", "/usr/bin", "hezhu/bin"};
    printf("%p\n", pm);
    //update_path(pm, strs,3);
    reset_path_manager(pm);
    print_paths(pm);

    char filename[128];
    strcpy(filename, "wish");
    char* f = PathManager_has(pm,filename );
    char buf[128];
    
    strcpy(buf, "wish");
    if(f!= NULL)
    {
        printf("It has file %s\n", f);
    }
    else
    {
        printf("It does not have file %s\n", f);
    }
    
    strcpy(buf, "PathManager");
    f = PathManager_has(pm, buf);
    if(f!= NULL)
    {
        printf("It has file %s\n", f);
    }
    else
    {
        printf("It does not have file %s\n", f);
    }

    destroy_path_manager(pm);
    printf("PathManager tested successfully\n");
}