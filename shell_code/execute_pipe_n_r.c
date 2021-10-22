#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include "execute_pipe_n_r.h"
#include "parse_input.h"

void execute_pipe_n_r(int in_cc,int out_cc,char* cmd1,char* cmd2,char* in_file,char* out_file,char** st_str,char** nd_arg,char** rd_arg,int arg_count)
{
  if(in_cc==1 && out_cc==0)
  {
    if(arg_count==2)
    {
      pid_t pid,pid2;
      int fd=open(in_file,O_RDONLY);
      int p[2];
      pipe(p);
      pid=fork();
      if(pid<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      if(pid==0)
      {
        close(p[0]);
        dup2(fd,0);
        dup2(p[1],1);
        char* st_arg[100];
        for(int i=0;i<100;i++)
          {
            st_arg[i]=NULL;
          }
        parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd);
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
    if(arg_count==3)
    {
      int p[2];
      int fd=open(in_file,O_RDONLY);
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
        dup2(fd,0);
        //close(p[1]);
        char* st_arg[100];
        for(int i=0;i<100;i++)
          {
            st_arg[i]=NULL;
          }
          parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0){
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd);
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
  if(in_cc==0 && out_cc==2)
  {
   if(arg_count==2)
    {
      pid_t pid,pid2;
      int fd=open(out_file,O_WRONLY| O_CREAT,0644);
      lseek(fd,0,SEEK_END);
      int p[2];
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
        if(execvp(st_str[0],st_str)<0)
        {
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
          dup2(fd,1);
          char* nd_str[100];
          for(int i=0;i<100;i++)
          {
            nd_str[i]=NULL;
          }
          parse_input(cmd2,nd_str);
          if(execvp(nd_str[0],nd_str)<0)
          {
            fprintf(stderr,"execvp -- error\n");
          }
          close(p[0]);
          close(fd);
          exit(0);
        }
        else
        {
          wait(NULL);
          return;
        }
      }
    }
    if(arg_count==3)
    {
      int p[2];
      int fd=open(out_file,O_WRONLY|O_CREAT,0644);
      lseek(fd,0,SEEK_END);
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
        if(execvp(st_str[0],st_str)<0){
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
            dup2(fd,1);
            char* rd_str[100];
            for(int i=0;i<100;i++)
            {
              rd_str[i]=NULL;
            }
            parse_input(cmd2,rd_str);
            if(execvp(rd_str[0],rd_str)<0)
            {
              fprintf(stderr,"execvp -- error\n");
            }
            close(p2[0]);
            close(fd);
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
  if(in_cc==0 && out_cc==1)
  {
    if(arg_count==2)
    {
      pid_t pid,pid2;
      int fd=open(out_file,O_WRONLY| O_CREAT| O_TRUNC,0644);
      int p[2];
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
        if(execvp(st_str[0],st_str)<0)
        {
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
          dup2(fd,1);
          char* nd_str[100];
          for(int i=0;i<100;i++)
          {
            nd_str[i]=NULL;
          }
          parse_input(cmd2,nd_str);
          if(execvp(nd_str[0],nd_str)<0)
          {
            fprintf(stderr,"execvp -- error\n");
          }
          close(p[0]);
          close(fd);
          exit(0);
        }
        else
        {
          wait(NULL);
          return;
        }
      }
    }
    if(arg_count==3)
    {
      int p[2];
      int fd=open(out_file,O_WRONLY|O_TRUNC|O_CREAT,0644);
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
        if(execvp(st_str[0],st_str)<0){
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
            dup2(fd,1);
            char* rd_str[100];
            for(int i=0;i<100;i++)
            {
              rd_str[i]=NULL;
            }
            parse_input(cmd2,rd_str);
            if(execvp(rd_str[0],rd_str)<0)
            {
              fprintf(stderr,"execvp -- error\n");
            }
            close(p2[0]);
            close(fd);
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
  if(in_cc==1 && out_cc==2)
  {
        if(arg_count==2)
    {
      pid_t pid,pid2;
      int fd[2];
      fd[0]=open(in_file,O_RDONLY);
      fd[1]=open(out_file,O_WRONLY| O_CREAT,0644);
      lseek(fd[1],0,SEEK_END);
      int p[2];
      pipe(p);
      pid=fork();
      if(pid<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      if(pid==0)
      {
        close(p[0]);
        dup2(fd[0],0);
        dup2(p[1],1);
        //close(p[1]);
        char* st_arg[100];
        for(int i=0;i<100;i++)
        {
          st_arg[i]=NULL;
        }
        parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd[0]);
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
          dup2(fd[1],1);
          char* nd_str[100];
          for(int i=0;i<100;i++)
          {
            nd_str[i]=NULL;
          }
          parse_input(cmd2,nd_str);
          if(execvp(nd_str[0],nd_str)<0)
          {
            fprintf(stderr,"execvp -- error\n");
          }
          close(p[0]);
          close(fd[1]);
          exit(0);
        }
        else
        {
          wait(NULL);
          return;
        }
      }
    }
    if(arg_count==3)
    {
      int p[2];
      int fd[2];
      fd[0]=open(in_file,O_RDONLY);
      fd[1]=open(out_file,O_WRONLY|O_TRUNC|O_CREAT,0644);
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
        dup2(fd[0],0);
        //close(p[1]);
        char* st_arg[100];
        for(int i=0;i<100;i++)
          {
            st_arg[i]=NULL;
          }
          parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0){
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd[0]);
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
            dup2(fd[1],1);
            char* rd_str[100];
            for(int i=0;i<100;i++)
            {
              rd_str[i]=NULL;
            }
            parse_input(cmd2,rd_str);
            if(execvp(rd_str[0],rd_str)<0)
            {
              fprintf(stderr,"execvp -- error\n");
            }
            close(p2[0]);
            close(fd[1]);
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
  if(in_cc==1 && out_cc==1)
  {
    if(arg_count==2)
    {
      pid_t pid,pid2;
      int fd[2];
      fd[0]=open(in_file,O_RDONLY);
      fd[1]=open(out_file,O_WRONLY| O_CREAT| O_TRUNC,0644);
      int p[2];
      pipe(p);
      pid=fork();
      if(pid<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      if(pid==0)
      {
        close(p[0]);
        dup2(fd[0],0);
        dup2(p[1],1);
        char* st_arg[100];
        for(int i=0;i<100;i++)
          {
            st_arg[i]=NULL;
          }
          parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd[0]);
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
          dup2(fd[1],1);
          char* nd_str[100];
          for(int i=0;i<100;i++)
          {
            nd_str[i]=NULL;
          }
          parse_input(cmd2,nd_str);
          if(execvp(nd_str[0],nd_str)<0)
          {
            fprintf(stderr,"execvp -- error\n");
          }
          close(p[0]);
          close(fd[1]);
          exit(0);
        }
        else
        {
          wait(NULL);
          return;
        }
      }
    }
    if(arg_count==3)
    {
      int p[2];
      int fd[2];
      fd[0]=open(in_file,O_RDONLY);
      fd[1]=open(out_file,O_WRONLY|O_TRUNC|O_CREAT,0644);
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
        dup2(fd[0],0);
        char* st_arg[100];
        for(int i=0;i<100;i++)
          {
            st_arg[i]=NULL;
          }
          parse_input(cmd1,st_arg);
        if(execvp(st_arg[0],st_arg)<0){
          fprintf(stderr,"execvp -- error\n");
        }
        close(p[1]);
        close(fd[0]);
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
            dup2(fd[1],1);
            char* rd_str[100];
            for(int i=0;i<100;i++)
            {
              rd_str[i]=NULL;
            }
            parse_input(cmd2,rd_str);
            if(execvp(rd_str[0],rd_str)<0)
            {
              fprintf(stderr,"execvp -- error\n");
            }
            close(p2[0]);
            close(fd[1]);
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
}
