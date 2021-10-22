#include<stdio.h>
#include<unistd.h>

char cwd[256];

char* pwd(void){
  if (getcwd(cwd, sizeof(cwd)) == NULL)
    perror("getcwd() error");
  else
    return cwd;
  return NULL;
}
