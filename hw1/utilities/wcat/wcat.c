//wcat.c
//HE ZHU , UT Austin, Advanced Operating Systems
//UT EID : HZ6627
//he.zhu@utexas.edu, he.zhu@uconn.edu
//Date: 02/02/2020
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


#define ERR_OPEN "wcat: cannot open file\n"

/*process reverse I/O here*/
/*@src: the file pointer we fread() from*/
void process_IO(FILE *src)
{
    char *line = NULL;
    ssize_t nread;
    size_t len;
    while ((nread = getline(&line, &len, src) )!= -1)
    {
        fprintf(stdout,"%s" ,line);
    }
}

/*safe fopen()*/
/*@filename: the filename we use to fopen()*/
/*@fp: file pointer as passed-in arg*/
/*@mode: the mode we use while calling fopen()*/
void Fopen(const char *filename, FILE **fp, const char *mode)
{
    if (!(*fp = fopen(filename, mode)) )
    {   
        fprintf(stdout, ERR_OPEN);
        exit(1);
    }
};

int main(int argc, char *argv[])
{


    /*@src: file pointer we use to read and write*/
    FILE *fp;

    if (argc == 1)
    {
        return 0;
    }
    else
    {
        for( int i = 1 ; i < argc ; i ++ )
        {
            Fopen(argv[i], &fp, "rb");
            process_IO(fp);
            fclose(fp);
        }
    }

    return 0;
}