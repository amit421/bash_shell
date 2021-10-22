#include<stdio.h>
#include<string.h>
#define MAX 100

void parse_input(char* input,char** parsed){
  

  for(int i=0;i<MAX;i++){
    parsed[i]=strsep(&input," \t");
    
    if(parsed[i]==NULL){ 
      parsed[i]='\0';
      break;
    }
    if(strlen(parsed[i])==0)i--;
  }

}