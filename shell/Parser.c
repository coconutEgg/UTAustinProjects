#include <stdlib.h>

#include "Parser.h"

int main(int argc, char *argv[])
{
    Parser ps;
    build_parser(&ps);
    char buff1[] = {"ls folder1>folder2>folder3"};
    assign_parser_buff(&ps, buff1);
    parser_swicth_buff(&ps, '>');
    show_parser_buff(&ps);

    reinit_parser(&ps);

    printf("\n");

    char buff2[] = {"ls folder1>file1&ls folder2>file2"};
    assign_parser_buff(&ps, buff2);
    parser_swicth_buff(&ps, '>');
    parser_swicth_buff(&ps, '&');
    show_parser_buff(&ps);

    parser_insert_arg("ls", &ps);
    parser_insert_arg("temp", &ps);
    parser_insert_arg(">", &ps);
    parser_insert_arg("temp2", &ps);

    printf("\n");

    show_parser_args(&ps);

    reinit_parser(&ps);
    show_parser_args(&ps);

    reinit_parser(&ps);
    assign_parser_buff(&ps, buff2);
    parser_swicth_buff(&ps, '>');
    parser_swicth_buff(&ps, '&');
    parser_buff_to_args(&ps, STDSEPS);
    show_parser_buff(&ps);
    printf("\n");
    show_parser_args(&ps);

    printf("\n");
    assign_parser_buff(&ps, "hello&world&>hello&world\n");
    parser_swicth_buff(&ps, '>');
    parser_swicth_buff(&ps, '&');
    parser_buff_to_args(&ps, STDSEPS);
    show_parser_buff(&ps);
    printf("\n");
    show_parser_args(&ps);

    while (1)
    {
        reinit_parser(&ps);
        char buff[BUFSZ];
        // print_paths(pm);
        printf("wish>");
        fgets(buff, sizeof(buff), stdin);
        parser_line(&ps, buff, STDSEPS,">&");
        printf("parser result: \n");
        show_parser_args(&ps);
    }
}