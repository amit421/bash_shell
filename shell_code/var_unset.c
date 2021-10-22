#include <stdio.h>
#include <string.h>
#include "var_unset.h"

extern char* env_var[1000];
extern int count;
void var_unset(char cmd[])
{ 
  int ck=0;
  char inp[600];
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
          for(int k=i;env_var[k-1]!=NULL;k++)
          {
            env_var[k]=env_var[k+1];
          }
          count--;
          break;
        }
    }
    for(int i=0; env_var[i]!=NULL;i++)
    {
        printf("%s\n",env_var[i]);
    }
}