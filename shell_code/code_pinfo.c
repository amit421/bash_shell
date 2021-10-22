#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include "parse_input.h"
#include<sys/types.h>
#include<signal.h>
#include<errno.h>

void code_pinfo(char** parsed){

  char o_one[]="/proc/";
  char o_two[]="/stat";
  char buf;
  char buffer[10000];
  int gg=0,gh=0;
  char *ans[100];
  int flag=0;
  
  if(!parsed[1]){
    char str[100];
    char more[100];
    long int p_pid=(long)getpid();
    sprintf(str,"%ld",p_pid);
    sprintf(more,"%ld",p_pid);
    printf("pid -- %ld\n",p_pid);

    strcat(o_one,str);
    strcat(o_one,o_two);
    int fd=open(o_one,O_RDONLY);
    
    
    while(1){
    read(fd,&buf,1);
    buffer[gh++]=buf;
    if(buf==' ')gg++;
    if(gg>23)break;
    }
    
    parse_input(buffer,ans); 
    printf("Process Status -- %s\n",ans[2]);
    printf("memory -- %s\n",ans[22]);
  
    char ne[]="/proc/";  
    char wo[]="/exe";
    char bufff[200];
    for(int p=0;p<200;p++){
      bufff[p]='\0';
    }
    strcat(ne,more);
    strcat(ne,wo);

    readlink(ne,bufff,100);
    printf("Executable Path -- %s\n",bufff);
    fflush(stdout);
    close(fd);
  }
  else
  {
    int n_name=atoi(parsed[1]);
    pid_t pid=n_name;
    kill(pid,0);
    if(errno==3)
    {
    flag=2;
    perror("error");
    }
    if(flag!=2)
    {
      printf("pid -- %d\n",n_name);
      strcat(o_one,parsed[1]);
      strcat(o_one,o_two);
      int fd=open(o_one,O_RDONLY);

      while(1)
      {
        read(fd,&buf,1);
        buffer[gh++]=buf;
        if(buf==' ')gg++;
        if(gg>23)break;
      }
      buffer[gh]='\0';
    
      parse_input(buffer,ans); 
      printf("Process Status -- %s\n",ans[2]);
      printf("memory -- %s\n",ans[22]);

      char ne[]="/proc/";  
      char wo[]="/exe";
      char buffff[200];
      for(int p=0;p<200;p++)
      {
        buffff[p]='\0';
      }
      strcat(ne,parsed[1]);
      strcat(ne,wo);
      readlink(ne,buffff,sizeof(buffff));
      printf("Executable Path -- %s\n",buffff);
      fflush(stdout);
      close(fd);
    }
  }
}
