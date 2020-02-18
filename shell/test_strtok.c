#include <stdio.h>
#include <string.h>

int main()
{
    char a[128] = "hell0 & world & hello";
    char *rest = NULL;
    char *token;

    for( token = strtok_r(a, "&", &rest); token != NULL ; token = strtok_r(NULL,"&",&rest))
    {
        printf("token:%s\n",token);
    }
}