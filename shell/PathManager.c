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
    destroy_path_manager(pm);

    printf("PathManager tested successfully\n");
}