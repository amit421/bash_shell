#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "var_set.h"

extern char *env_var[1000];
extern int count;
void var_set(char cmd[],char input[])
{
    int ck=0,gk=0;
    int flag=0;
    char inp[100];

    char s_str[100];
    for(int i=0;i<strlen(cmd);i++)
    {
      s_str[i]=cmd[i];
      gk++;
    }
    s_str[gk]='\0';
    strcat(s_str,"=");
    strcat(s_str,input);

    for(int i=0;env_var[i]!=NULL;i++)
    {
        ck=0;
        for(int j=0;j<strlen(env_var[i]);j++)
        {
            if(env_var[i][j]=='=')
            {
                break;
            }
            inp[ck++]=env_var[i][j];
        }
        inp[ck]='\0';
        if(!strcmp(cmd,inp))
        {
            env_var[i]=strdup(s_str);
            flag=1;
        }
    }
    if(flag!=1)
    {
        env_var[count++]=strdup(s_str);
    }

    /*for(int i=0; env_var[i]!=NULL;i++)
    {
        printf("%s\n",env_var[i]);
    }*/
}