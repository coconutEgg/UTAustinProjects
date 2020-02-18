#ifndef PARSER_H_
#define PARSER_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSZ 4096 //The maximum size of a string
#define ARGCNT 64  //The maximum ARGCNT
#define ARGLEN 256 //The maximum single arg len
#define SMBUFSZ 128
#define STDSEPS " \r\n\t"
#define STDOPRS ">&"

typedef struct _Parser Parser;
void build_parser(Parser *ps);
void destroy_parser(Parser *ps);
void reinit_parser(Parser *ps);
void parser_insert_arg(const char *buff, Parser *ps);
void assign_parser_buff(Parser *ps, const char *buff);
void parser_swicth_buff(Parser *ps, const char seperator);
void parser_buff_to_args(Parser *ps, const char *seps);
void show_parser_args(Parser *ps);
void show_parser_buff(Parser *ps);


typedef struct _Parser
{
    int buf_switched;
    int argcnt;
    char *args[ARGCNT]; //Points to array of args
    char buff[BUFSZ];
} Parser;


void build_parser(Parser *ps)
{
    ps->argcnt = 0;
    ps->buf_switched = 0;
}

void destroy_parser(Parser *ps)
{
    for (int i = 0; i < ps->argcnt; i++)
    {
        free(ps->args[i]);
        ps->args[i] = NULL;
    }
    ps->argcnt = 0;
    ps->buf_switched = 0;
}

void reinit_parser(Parser *ps)
{
    if (ps != NULL)
    {
        destroy_parser(ps);
    }
    build_parser(ps);
}

void assign_parser_buff(Parser *ps, const char *buff)
{
    strcpy(ps->buff, buff);
}

void parser_swicth_buff(Parser *ps, const char seperator)
{
    if (!sizeof(ps->buff))
    {
        return;
    }

    int i = 0, j = 0;
    char _buff[BUFSZ];
    memset(_buff, 0, BUFSZ);

    while (i < BUFSZ && j < BUFSZ)
    {
        if (ps->buff[i] != seperator)
        {
            _buff[j] = ps->buff[i];
            j++;
            i++;
        }
        if (ps->buff[i] == seperator)
        {
            _buff[j] = ' ';
            j++;
            _buff[j] = ps->buff[i];
            j++;
            i++;
            _buff[j++] = ' ';
        }
    }
    _buff[strlen(_buff)] = '\0';
    memset(ps->buff, 0, sizeof(ps->buff));
    strcpy(ps->buff, _buff);
    ps->buf_switched = 1;
}

void parser_buff_to_args(Parser *ps, const char *seps)
{
    char buff[BUFSZ];
    strcpy(buff, ps->buff);
    if (!ps->buf_switched)
    {
        printf("buff must be switched first\n");
        return;
    }

    int i = 0;
    char *pch = strtok(ps->buff, seps);
    while (pch != NULL)
    {
        parser_insert_arg(pch, ps);
        pch = strtok(NULL, seps);
        i++;
    }
    strcpy(ps->buff, buff);
}

void parser_insert_arg(const char *buff, Parser *ps)
{
    ps->args[ps->argcnt] = (char *)malloc(ARGCNT);
    strcpy(ps->args[ps->argcnt], buff);
    ps->argcnt++;
}

void show_parser_args(Parser *ps)
{
    for (int i = 1; i <= ps->argcnt; i++)
    {
        printf("arg(%d): %s\t", i, ps->args[i - 1]);
    }
    printf("\n");
}

void show_parser_buff(Parser *ps)
{
    printf("%s", ps->buff);
}

void parser_batch(Parser *ps, int argc, char *args[], const char *switch_chars)
{
    reinit_parser(ps); //re-initialize the parser every time
    for (int i = 0; i < argc; i++)
    {
        //(1) assign the buff
        assign_parser_buff(ps, args[i]);
        int j = 0;
        //(2) swithc the buff
        while (switch_chars[j] != '\0')
        {
            parser_swicth_buff(ps, switch_chars[j++]);
        }
        //(3) convert buff to args.
        parser_buff_to_args(ps, STDSEPS);
    }
}

void parser_line(Parser *ps, char *_line, const char *seperators, const char *swith_chars)
{
    reinit_parser(ps);
    char line[strlen(_line) + 1];
    strcpy(line, _line);
    assign_parser_buff(ps, line);
    int i = 0;
    while (swith_chars[i] != '\0')
    {
        parser_swicth_buff(ps, swith_chars[i++]);
    }
    parser_buff_to_args(ps, seperators);
}
#endif