#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void switch_space(char *pbuf)
{   
    
    int i = 0;
    int j = 0;
    int l1 = strlen(pbuf);
    int l2 = 2 * l1;
    
    char *_pbuf = (char*)malloc(l2);;
    
    printf("%d\n", l1);
    printf("%d\n", l2);

    while (i < l1 && j < l2 )
    {   
        //printf("--");
        if(pbuf[i] != '>' && pbuf[i] != '&')
        {
            _pbuf[j] = pbuf[i];
            j++;
            i++;
        }
        if(pbuf[i] == '>' || pbuf[i] == '&')
        {
            _pbuf[j] = ' ';
            j++;
            _pbuf[j] =pbuf[i];
            j ++;
            i ++;
            _pbuf[j++] = ' ';
        }
    }
    _pbuf[strlen(_pbuf)] ='\0';
    printf("%s", _pbuf);
}

int main()
{
    char buff[] = "ls tests/p2a-test>/tmp/output11";
    switch_space(buff);
    // printf("%s", buff);
}