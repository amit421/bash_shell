#include "rm_space.h"
void rm_space(char * str)
{
    int index, i;

    index = 0;

    while(str[index] == ' ' || str[index] == '\t')
    {
        index++;
    }
    if(index != 0)
    {
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0';
    }
}