#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void code_exit(char* history[20],int curr){

  char username[256];
  char a[100];
  getlogin_r(username,sizeof(username));
  int co=0,po=0;
  char home[100]="/home/";
  home[7]='\0';
  for(int i=0;i<strlen(username);i++){
    home[6+i]=username[i];
    co++;
  }
  home[co+6]='\0';
  for(int i=0;i<strlen(home);i++){
    a[i]=home[i];
    po++;
  }
  a[po]='\0';
  char* b="/history.txt";
  strcat(a,b);

  int fd=open(a,O_CREAT | O_TRUNC | O_RDWR,0600);
  lseek(fd,0,SEEK_END);
  int i=curr;

  do{
      if(history[i]){
       // printf(" %s\n",history[i]);
        char *a_arr=strdup(history[i]);
        strcat(a_arr,"\n");
        write(fd,a_arr,strlen(a_arr));
      }
      i=(i+1)%20;
    }while(i!=curr);

  close(fd);
  printf("exiting\n");
  exit(0);
}