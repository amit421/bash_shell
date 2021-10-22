#include "rm_t_space.h"
void rm_t_space(char * str)
{
    int index, i;

    index = -1;
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t')
        {
            index= i;
        }

        i++;
    }
    str[index + 1] = '\0';
}