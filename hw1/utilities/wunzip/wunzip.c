#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>

void unzip(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("wunzip: cannot open file\n");
    }

    int count = 0;
    char buff[5];
    while (fread(&count, 4, 1, fp))
    {
        memset(buff, 0, strlen(buff));
        fread(buff, 1, 1, fp);
        for (size_t i = 0; i < count; i++)
        {
            printf("%s", buff);
        }
    }

    fclose(fp);
}

    int main(int argc, char *argv[])
    {
        if (argc == 1)
        {
            fprintf(stdout, "wunzip: file1 [file2 ...]");
            exit(1);
        }
        else
        {
            for (int i = 1; i < argc; i++)
            {
                unzip(argv[i]);
            }
        }

        return 0;
    }
