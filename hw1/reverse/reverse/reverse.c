
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

/*@BUFSZ : control the maximum char buffer size*/
#define BUFSZ 512

#define ERR_RWSAME "reverse: input and output file must differ\n"
#define ERR_WRFILE "reverse: cannot open file '/no/such/file.txt'\n"
#define ERR_USAGE "usage: reverse <input> <output>\n"
#define ERR_MALLOC "err: malloc() error"
/*@buff: the memory allocated in the stack for store the chars*/
/*@next: the pointer pointing to the next block*/
/*@prev: the pointer pointing to the previous block*/
typedef struct _block_t
{
    char *buff;
    struct _block_t *next;
    struct _block_t *prev;

} block_t;

void destroy_block(block_t *blk)
{
    free(blk->buff);
    blk->next = NULL;
    blk->prev = NULL;
}

/*process reverse I/O here*/
/*@src: the file pointer we fread() from*/
/*@dest: the file pointer we write() to*/
void process_IO(FILE *src, FILE *dest)
{
    //create and initialize the block to be NULL
    block_t *blk = malloc(sizeof(struct _block_t));
    if (!blk)
    {
        fprintf(stderr, ERR_MALLOC);
        exit(1);
    }

    blk->prev = NULL;

    char *line = NULL;
    ssize_t nread;
    size_t len;
    while ((nread = getline(&line, &len, src) )!= -1)
    {
        blk->buff = malloc(strlen(line) + 1);
        if (!blk)
        {
            fprintf(stderr, ERR_MALLOC);
            exit(1);
        }
        strcpy(blk->buff, line);

        // printf("%s", blk->buff);
        block_t *nxt_blk = malloc(sizeof(struct _block_t));
        blk->next = blk;
        nxt_blk->prev = blk;
        blk = nxt_blk;
    }
    blk = blk->prev;
    block_t *last = blk;

    while (blk)
    {
        // printf("%s", blk->buff);
        fprintf(dest,"%s",blk->buff);
        blk = blk->prev;
    }

    while (blk)
    {
        destroy_block(blk);
    }
    destroy_block(last);
}

/*safe fopen()*/
/*@filename: the filename we use to fopen()*/
/*@fp: file pointer as passed-in arg*/
/*@mode: the mode we use while calling fopen()*/
void Fopen(const char *filename, FILE **fp, const char *mode)
{
    if (!(*fp = fopen(filename, mode)) )
    {   
        fprintf(stderr, ERR_WRFILE);
        exit(1);
    }
};

int is_same_file(const char *file1, const char *file2)
{
    if(strcmp(file1, file2) == 0 )
    {
        return 0;
    }

    struct stat sb ={0};
    // memset(&sb, 0, sizeof(sb));
    int ret = stat(file1, &sb);

    if(ret == -1)
    {
        fprintf(stderr, ERR_WRFILE);
        exit(1);
    }
    ino_t ino1 = sb.st_ino;

    ret = stat(file2, &sb);
    if(ret == -1)
    {
        fprintf(stderr, ERR_WRFILE);
        exit(1);
    }
    
    ino_t ino2 = sb.st_ino;
    
    if(ino1 == ino2)
    {
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, ERR_USAGE);
        exit(1);
    }

    /*@src: file pointer we use to read and write*/
    FILE *src;
    FILE *dest;

    if (argc == 1)
    {
        src = stdin;
        dest = stdout;
    }
    else if (argc == 2)
    {
        Fopen(argv[1], &src, "rb");
        dest = stdout;
    }
    else
    {
        Fopen(argv[1], &src, "r+b");
        Fopen(argv[2], &dest, "w+b");

        if ( is_same_file(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, ERR_RWSAME);
            exit(1);
        }

    }

    process_IO(src, dest);
    fclose(src);
    fclose(dest);
}
