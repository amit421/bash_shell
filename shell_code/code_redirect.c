#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include "pwd.h"
void code_redirect(int in_check_flag, int out_check_flag,char in_file[],char out_file[],char **parsed){

int fd[2];

if(out_check_flag==2 && in_check_flag==0)
{
    fd[1]=open(out_file,O_WRONLY|O_CREAT,0644);
    lseek(fd[1],0,SEEK_END);
    pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"fork failed\n");
    }
    if(pid==0)
    {
        dup2(fd[1],1);
        if(execvp(parsed[0],parsed)<0)
        {
            fprintf(stderr,"execvp --error\n");
        }
    }
    else
    {
        wait(NULL);
        close(fd[1]);
        return;
    }
}
if(out_check_flag==2 && in_check_flag==1)
{
    fd[0]=open(in_file,O_RDONLY);
    fd[1]=open(out_file,O_CREAT | O_WRONLY,0644);
    lseek(fd[1],0,SEEK_END);
    pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"fork failed\n");
    }
    if(pid==0){
        dup2(fd[0],0);
        dup2(fd[1],1);
        if(execvp(parsed[0],parsed)<0){
            fprintf(stderr,"execvp -- error\n");
        }
        exit(0);
    }
    else{
        wait(NULL);
        close(fd[0]);
        close(fd[1]);
        return;
    }
}
if(out_check_flag==0 && in_check_flag==1)
{
    fd[0]=open(in_file,O_RDONLY);
    pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"fork failed\n");
    }
    if(pid==0){
        dup2(fd[0],0);
        if(execvp(parsed[0],parsed)<0)
        {
            fprintf(stderr,"execvp -- error\n");
        }
        exit(0);
    }
    else{
        wait(NULL);
        close(fd[0]);
        return;
    }
}
if(out_check_flag==1 && in_check_flag==0)
{
    fd[1]=open(out_file,O_CREAT |O_TRUNC| O_WRONLY,0644);
    __pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"fork failed\n");
    }
    if(pid==0){
        dup2(fd[1],1);
        if(execvp(parsed[0],parsed)<0){
            fprintf(stderr,"execvp -- error\n");
        }
        exit(0);
    }
    else{
        wait(NULL);
        close(fd[1]);
        return;
    }
}
if(out_check_flag==1 && in_check_flag==1)
{
    fd[0]=open(in_file,O_RDONLY);
    fd[1]=open(out_file,O_CREAT |O_TRUNC| O_WRONLY,0644);
    pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"fork failed\n");
    }
    if(pid==0){
        dup2(fd[0],0);
        dup2(fd[1],1);
        if(execvp(parsed[0],parsed)<0){
            fprintf(stderr,"execvp -- error\n");
        }
        exit(0);
    }
    else{
        wait(NULL);
        close(fd[0]);
        close(fd[1]);
        return;
    }
}

}