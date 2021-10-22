#include<stdio.h>
#include<unistd.h>
#include<string.h>
#define MAX 100

char arr[100];

void code_echo(char** parsed){
  int size=0;
    for(int i=1;i<MAX;i++){
      if(parsed[i]==NULL)break;
      else size++;
    }
    for(int i=1;i<MAX;i++){
      if(parsed[i]==NULL)break;

      if(parsed[i][0]=='"'){
        //printf("5");
       for(int j=1;j<sizeof(parsed[1]);j++){
          arr[j-1]=parsed[1][j];
        }
        printf("%s ",arr);
      }

      else if(parsed[i][strlen(parsed[i])-1]=='"'){
        for(int k=0;k<strlen(parsed[size])-1;k++){
          arr[k]=parsed[size][k];
        }
        for(int i=0;i<strlen(parsed[size])-1;i++){
          printf("%c",arr[i]);
        }
      }
    else{
    printf("%s ",parsed[i]);
    }
    }
    printf("\n");
}