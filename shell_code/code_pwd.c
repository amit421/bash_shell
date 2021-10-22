#include<stdio.h>
#include "pwd.h"

void code_pwd(void){
  char*val=pwd();
  printf("%s\n",val);
}