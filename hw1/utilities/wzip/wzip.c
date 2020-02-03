//wzip.c
//HE ZHU , UT Austin, Advanced Operating Systems
//UT EID : HZ6627
//he.zhu@utexas.edu, he.zhu@uconn.edu
//Date: 02/02/2020
#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MAXRLEN 128
#define MAXSZ 8192

//classical running length encoding code
/*@src: the src line we want to compress*/
void zip(char *src)
{
    //running length
   unsigned int rlen;
   char count[MAXRLEN];
   unsigned int len = strlen(src);

   //char *dest = (char*)malloc(sizeof(char)*(len*2+1));

   unsigned int i = 0, j = 0, k;

   for(i = 0 ; i < len ; i++ )
   {
       rlen = 1;
       char temp = src[i];
       while( i + 1 < len && src[i] == src[i+1] )
       {
           rlen ++;
           i++;
       }

        fwrite(&rlen, 4, 1, stdout);
        //fprintf(stdout,"%d",rlen);
        // sprintf(count, "%d", rlen);
        for(k = 0 ; *(count+k); k ++, j++ )
        {
            //dest[j] = count[k];
        }
        fwrite(&temp, 1, 1 ,stdout);
        //fprintf(stdout, "%c", temp);
        //dest[j++] = temp;
   }
   //dest[j] ='\0';

}

int main(int argc, char *argv[])
{   
    FILE *fp;
    FILE *temp = fopen("temp.txt", "w+");
    if(temp == NULL)
    {
        fprintf(stdout,"%s","wzip: file1 [file2 ...]");
        exit(1);
    }
    
    char *_mmap;

    if(argc == 1)
    {
        fprintf(stdout,"%s","wzip: file1 [file2 ...]");
        exit(1);
    }
    else
    {   
        //merge multiple files first
        for(int i = 1; i < argc; i ++ )
        {
            fp = fopen(argv[i], "rb");
            if(fp == NULL)
            {
                printf("wzip: file1 [file2 ...]");
                // exit(1);
                return 1;
            }
            char c;
            while( (c = fgetc(fp)) != EOF )
            {
                fputc(c, temp);
            }
            
            fclose(fp);
        }
        fclose(temp);
        //use mmap, for simplicity, we use buffer
        int fd = open("temp.txt", O_RDONLY, 0);
    
        struct stat st;
        stat("temp.txt", &st);
        size_t filesz = st.st_size;
        _mmap = (char *)mmap(NULL, filesz, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
        
        if(_mmap == (void*)-1)
        {
            exit(1);
        }
    }

     zip(_mmap);
     system("rm -rf test.txt");
    //fwrite(dest, sizeof(char) ,sizeof(dest), stdout);
    //fprintf(stdout,"%s",dest);
    return 0;
}