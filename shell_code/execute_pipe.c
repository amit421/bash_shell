#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include "execute_pipe.h"

void execute_pipe(char** st_arg,char** nd_arg,char** rd_arg,int arg_count)
{
  if(arg_count==2)  //for 1 pipe
  {
    int p[2];
    pid_t pid,pid2;
    pipe(p);
    pid=fork();
    if(pid<0)
    {
      fprintf(stderr,"fork failed\n");
    }
    if(pid==0)
    {
      close(p[0]);
      dup2(p[1],1);
      if(execvp(st_arg[0],st_arg)<0){
        fprintf(stderr,"execvp -- error\n");
      }
      close(p[1]);
      exit(0);
    }
    else
    {
      wait(NULL);
      close(p[1]);
      pid2=fork();
      if(pid2<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      if(pid2==0)
      {
        dup2(p[0],0);
        if(execvp(nd_arg[0],nd_arg)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[0]);
        exit(0);
      }
      else
      {
        wait(NULL);
        return;
      }
      
    }
  }
  if(arg_count==3)      //for 2 pipes
  {

    int p[2];
    pid_t pid,pid2,pid3;
    pipe(p);
    pid=fork();
    if(pid<0)
    {
      fprintf(stderr,"fork failed\n");
    }
    if(pid==0)
    {
      close(p[0]);
      dup2(p[1],1);
      if(execvp(st_arg[0],st_arg)<0){
        fprintf(stderr,"execvp -- error\n");
      }
      close(p[1]);
      exit(0);
    }
    else
    {
      wait(NULL);
      close(p[1]);
      int p2[2];
      pipe(p2);
      pid2=fork();
      if(pid2<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      if(pid2==0)
      {
        close(p2[0]);
        dup2(p[0],0);
        dup2(p2[1],1);
        if(execvp(nd_arg[0],nd_arg)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[0]);
        close(p2[1]);
        exit(0);
      }
      else
      {
        wait(NULL);
        close(p2[1]);
        pid3=fork();
        if(pid3<0)
        {
          fprintf(stderr,"fork failed\n");
        }
        if(pid3==0)
        {
          dup2(p2[0],0);
          if(execvp(rd_arg[0],rd_arg)<0)
          {
            fprintf(stderr,"execvp -- error\n");
          }
          close(p2[0]);
          exit(0);
        }
        else
        {
          wait(NULL);
          return;
        }
      }
    }
  }
}