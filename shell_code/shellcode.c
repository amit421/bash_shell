#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<wait.h>
#include<fcntl.h>
#include<signal.h>
#include<time.h>
#define MAX 100
#include "execute_pipe_n_r.h"
#include "execute_pipe.h"
#include "rm_space.h"
#include "rm_t_space.h"
#include "code_ls.h"
#include "code_exit.h"
#include "code_pwd.h"
#include "code_pinfo.h"
#include "code_echo.h"
#include "code_cd.h"
#include "parse_input.h"
#include "pwd.h"
#include "code_redirect.h" 
#include "var_set.h"
#include "var_unset.h"
#define clear() printf("\033[H\033[J")

char cwd[256];
char h_name[1000];
char *line=NULL;
char arr[100];
char *uname;
int ko=0;
char *env_var[1000];
long long int count=0;
long int con=0;
char *history[20];
int curr=0;
int checko=0;
int pipe_arg_count=0;
int i_r_count=0;
int i_r_count2=0;
int o_r_count=0;
int o_r_count2=0;
int dlg=0;
char str[100];
char str_1[100];
long long int pid_count=0;
pid_t child_arr[1000];
char *child_name[1000];
int mark[1000];
pid_t sig_pid=-1;
char naam[500];

void handle(int signum)
{
  if(sig_pid>0)
  {
    if(signum==20)
    {
      child_arr[pid_count]=sig_pid;
      child_name[pid_count++]=strdup(naam);
    }
    kill(sig_pid,signum);
    sig_pid=-1;
  }

}

void delay(int seconds) 
{ 
    int ms = 1000 * seconds;  
    clock_t start_time = clock();  
    while (clock() < start_time + ms); 
}

void rm_kill(void)
{
  int count=0;
  for(int i=0;i<pid_count;i++)
  {
    if(mark[i]==1)
    {
      count++;
    }
    mark[i]=0;
  }
  if(count==pid_count)
  {
    pid_count=0;
  }

}
void overkill_jobs(void)
{
  for(int i=0;i<pid_count;i++)
  {
    mark[i]=1;
    kill(child_arr[i],9);
  }
  rm_kill();
}
void k_job(char arr1[],char arr2[])
{
  int id=atoi(arr1);
  int sig=atoi(arr2);
  if(mark[id-1]==1 || id-1<0 || id>pid_count)
  {
    fprintf(stderr,"process doesn't exist\n");
  }
  else
  {
    kill(child_arr[id-1],sig);
  }
}
void bg_job(char arr[])
{
  int flag=0;
  int job_no=atoi(arr);
  if(mark[job_no-1]==1)
  {
    fprintf(stderr,"process doesn't exist\n");
  }
  else
  {
    int fd;
    char buf;
    char buffer[100];
    int gh=0,gg=0;
    char *aa_ans[10];

    char one[100]="/proc/";
    char two[100]="/stat";
    one[6]='\0';
    two[5]='\0';
    sprintf(str,"%d",child_arr[job_no-1]);
    gh=0,gg=0;
    strcat(one,str);
    strcat(one,two);

    fd=open(one,O_RDONLY);
    if(fd<0)
    {
      flag=1;
      fprintf(stderr,"file doesn't exist\n");
    }
    if(flag!=1)
    {
      while(1)
      {
        read(fd,&buf,1);
        buffer[gh++]=buf;
        if(buf==' ')gg++;
        if(gg>5)break;
      }
      parse_input(buffer,aa_ans);
      if(!strcmp(aa_ans[2],"T"))
      {
        kill(child_arr[job_no-1],18);      
      }
    }
  }
}
void fg_job(char arr[])
{
  int status;
  int job_no=atoi(arr);
  printf("%s\n",child_name[job_no-1]);
  
  tcsetpgrp(0,child_arr[job_no-1]);
  signal(SIGTTOU,SIG_IGN);
  waitpid(child_arr[job_no-1]-1,&status,WUNTRACED);
  tcsetpgrp(0,getpid());
  signal(SIGTTOU,SIG_DFL);

  mark[job_no-1]=1;
}
void print_jobs(void)
{
  
  int fd;
  char buf;
  char buffer[100];
  int gh=0,gg=0;
  char *aa_ans[10];

  rm_kill();
  for(int i=0;i<pid_count;i++)
  {
    char one[100]="/proc/";
    char two[100]="/stat";
    one[6]='\0';
    two[5]='\0';
    sprintf(str,"%d",child_arr[i]);
    gh=0,gg=0;
    strcat(one,str);
    strcat(one,two);

    fd=open(one,O_RDONLY);
    if(fd<0)
    {
      if(mark[i]!=1)mark[i]=1;
      continue;
    }
    while(1)
    {
      read(fd,&buf,1);
      buffer[gh++]=buf;
      if(buf==' ')gg++;
      if(gg>5)break;
    }
    
    parse_input(buffer,aa_ans);
    if(!strcmp(aa_ans[2],"S"))
    {
      printf("[%d] Running %s [%d]\n",i+1,child_name[i],child_arr[i]);
    }
    else if(!strcmp(aa_ans[2],"R"))
    {
        printf("[%d] Running %s [%d]\n",i+1,child_name[i],child_arr[i]);      
    }
    else if(!strcmp(aa_ans[2],"T"))
    {
        printf("[%d] Stopped %s [%d]\n",i+1,child_name[i],child_arr[i]);      
    }
    else if(!strcmp(aa_ans[2],"Z"))
    {
        printf("[%d] Zombie %s [%d]\n",i+1,child_name[i],child_arr[i]);
    }
    else if(strlen(aa_ans[2])==0)
    {
      if(mark[i]!=1)mark[i]=1;
    }
    
    close(fd);
    for(int i=0;i<100;i++)
    {
      one[i]='\0';
    }
    for(int i=0;i<100;i++)
    {
      buffer[i]='\0';
      one[i]='\0';
      two[i]='\0';
      str[i]='\0';
    }
  }
}

void file_read(int fd)
{
  char ch;
  int k=0,o=0;
  char arr[900];
  long int val=lseek(fd,0,SEEK_END);
  lseek(fd,-val,SEEK_CUR);
  for(int i=0;i<val;i++)
  {
    read(fd,&ch,1);
    arr[o++]=ch;
    if(ch=='\n')
    {
      arr[o-1]='\0';
      history[k++]=strdup(arr);
      for(int j=0;j<o;j++)
      {
        arr[j]='\0';
      }
      o=0;
    }
  }
  checko=(k%20);
  if(k<20)
  {
    for(int l=k;l<20;l++)
    {
      history[l]=NULL;
    }
  }
}

void na_name(char* username,char arr[]){
  int co=0,po=0;
  char home[100]="/home/";
  home[7]='\0';
  for(int i=0;i<strlen(username);i++){
    home[6+i]=username[i];
    co++;
  }
  home[co+6]='\0';
  for(int i=0;i<strlen(home);i++){
    arr[i]=home[i];
    po++;
  }
  arr[po]='\0';
}

void code_history(int gh){
  int i=((20-gh)+curr)%20;
  do{
      if(history[i]){
        printf(" %s\n",history[i]);
      }
      i=(i+1)%20;
    }while(i!=curr);
}


void final_strs(char* input,char** parsed,int* arg2_count)
{
    for(int i=0;i<100;i++)
    {
        //printf("input %s\n",input);
        parsed[i]=strsep(&input," ");
        //printf("parsed %s\n",parsed[i]);
        if(parsed[i]==NULL)
        {
            break;
        }
        else{
            *arg2_count=*arg2_count+1;
        }
        if (strlen(parsed[i])==0)i--;
        
    }
}

void i_r_parse2(char* input, char** parsed)
{
    i_r_count2=0;
    for(int i=0;i<100;i++)
    {
        parsed[i]=strsep(&input," ");
        if(parsed[i]==NULL)
        {
            break;
        }
        else if(strcmp(parsed[i],"\0")!=0)
        {
            i_r_count2++;
        }   
        
        if(strlen(parsed[i])==0)i--;
    }
}

void i_r_parse(char* input, char** parsed)
{
    i_r_count=0;
    for(int i=0;i<100;i++)
    {
        parsed[i]=strsep(&input,"<");
        if(parsed[i]==NULL)
        { 
            break;
        }
        else 
        {
            i_r_count++;
        }   
        
        if(strlen(parsed[i])==0)i--;
    }
}

void o_r_parse2(char* input, char** parsed)
{
    o_r_count2=0;
    for(int i=0;i<100;i++)
    {
        parsed[i]=strsep(&input," ");
        if(parsed[i]==NULL)
        {
            break;
        }
        else if(strcmp(parsed[i],"\0")!=0)
        {
            o_r_count2++;
        }   
        
        if(strlen(parsed[i])==0)i--;
    }
}

void o_r_parse(char* input, char** parsed)
{
    o_r_count=0;
    for(int i=0;i<100;i++)
    {
        parsed[i]=strsep(&input,">");
        if(parsed[i]==NULL)
        { 
            break;
        }
        else if(strcmp(parsed[i],"\0")!=0)
        {
            o_r_count++;
        }   
        
        if(strlen(parsed[i])==0)i--;
    }
}


int has_pipe(char *input,char** pipe_parsed){
  pipe_arg_count=0;
  for(int i=0;i<100;i++){
    pipe_parsed[i]=strsep(&input,"|");
    if(pipe_parsed[i]==NULL){
      break;
    }
    else{
    pipe_arg_count++;
    }
    if(strlen(pipe_parsed[i])==0)i--;
  }
  
  if(pipe_arg_count==1)
  {
    return 0;
  }
  if(pipe_parsed[pipe_arg_count-1]==NULL){
    return 0;
  }
  return 1;
} 

int has_input_redirection(char* parsed){
  for(int i=0;i<strlen(parsed);i++){
    if(parsed[i]=='<'){
      return 1;
    }
  }
  return 0;
}

int has_output_redirection(char* parsed){
  for(int i=0;i<strlen(parsed);i++){
    if(parsed[i]=='>'){
      return 1;
    }
  }
  return 0;
}

int has_append(char* parsed)
{
  for(int i=0;i<strlen(parsed);i++)
    {
        if(parsed[i]=='>')
        {
            char arra[3]={parsed[i],parsed[i+1],'\0'};
          
          if(!strcmp(arra,">>"))
          {
            return 1;
          }
        }
    }
    return 0;
}

void args_exec(char* input,char** parsed){
  int c_count=0;
  char out_file[80];
  char in_file[80];
  char cmd1[200];
  char cmd2[200];
  int ll=0;
  int imp_flag=0;
  char* pipe_parsed[30];
  int in_cc=0;
  int out_cc=0;
  for(int i=0;i<MAX;i++)
  {
    parsed[i]=NULL;
  }

  if(has_pipe(input,pipe_parsed))
  {

    for(int i=0;i<pipe_arg_count;i++)
    {
      rm_space(pipe_parsed[i]);
      rm_t_space(pipe_parsed[i]);
    }

    in_cc=0;
    out_cc=0;
    if(has_input_redirection(pipe_parsed[0]))
    {
      in_cc=1;
    }
    if(has_output_redirection(pipe_parsed[pipe_arg_count-1]))
    {
      out_cc=1;
    }
    if(has_append(pipe_parsed[pipe_arg_count-1]))
    {
      out_cc=2;
    }

    if(in_cc==0 && out_cc==0)
    {
      for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* st_str[100];
      char* nd_str[100];
      char* rd_str[100];
      int arg2_count_1=0;
      int arg2_count_2=0;
      int arg2_count_3=0;
      for(int i=0;i<pipe_arg_count;i++)
      {
        if(i==0)
        final_strs(pipe_parsed[i],st_str,&arg2_count_1);
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
        if(i==2)
        final_strs(pipe_parsed[i],rd_str,&arg2_count_3);
      }
      execute_pipe(st_str,nd_str,rd_str,pipe_arg_count);
    }



    if(in_cc==1 && out_cc==0)
    {
      for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* nd_str[100];
      char* rd_str[100];
      int arg2_count_2=0;
      int arg2_count_3=0;
      for(int i=1;i<pipe_arg_count;i++)
      {
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
        if(i==2)
        final_strs(pipe_parsed[i],rd_str,&arg2_count_3);
      }
      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[0]);i++)
      {
        if(pipe_parsed[0][i]=='<')
        {
          int ki=0;
          for(int j=i+1;j<strlen(pipe_parsed[0]);j++)
          {
            in_file[ki++]=pipe_parsed[0][j];
          }
          in_file[ki]='\0';         //inputfile
          rm_space(in_file);
          rm_t_space(in_file);
          break;
        }
        cmd1[ll++]=pipe_parsed[0][i];
      }
      cmd1[ll]='\0';
      rm_space(cmd1);
      rm_t_space(cmd1);
      dlg=0;
      if( access(in_file, F_OK ) != -1 )
      {
        // file exists
      } 
      else
      {
        dlg=1;    // file doesn't exist
      }
      if(!dlg)
      { 
        execute_pipe_n_r(in_cc,out_cc,cmd1,NULL,in_file,NULL,NULL,nd_str,rd_str,pipe_arg_count);
      }
      else
      {
        fprintf(stderr,"input file error\n");
      }
      
    }
    if(in_cc==0 && out_cc==2)
    {
     for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* nd_str[100];
      char* st_str[100];
      int arg2_count_2=0;
      int arg2_count_1=0;
      for(int i=0;i<pipe_arg_count-1;i++)
      {
        if(i==0)
        final_strs(pipe_parsed[i],st_str,&arg2_count_1);
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
      }
      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[pipe_arg_count-1]);i++)
      {
        if(pipe_parsed[pipe_arg_count-1][i]=='>')
        {
          char arraa[3]={pipe_parsed[pipe_arg_count-1][i],pipe_parsed[pipe_arg_count-1][i+1],'\0'};
          if(!strcmp(arraa,">>"))
          {
            int ki=0;
            for(int j=i+2;j<strlen(pipe_parsed[pipe_arg_count-1]);j++)
            {
              out_file[ki++]=pipe_parsed[pipe_arg_count-1][j];
            }
            out_file[ki]='\0';          //outputfile
            rm_t_space(out_file);
            rm_space(out_file);
            break;
          }
        }
        cmd2[ll++]=pipe_parsed[pipe_arg_count-1][i];
      }
      cmd2[ll]='\0';
      rm_space(cmd2);
      rm_t_space(cmd2);

     if(strlen(out_file)==0)
     {
      fprintf(stderr,"output file not provided\n");
     }
     else
     {
      execute_pipe_n_r(in_cc,out_cc,NULL,cmd2,NULL,out_file,st_str,nd_str,NULL,pipe_arg_count);
     }
    }

    if(in_cc==0 && out_cc==1)
    {
      for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* nd_str[100];
      char* st_str[100];
      int arg2_count_2=0;
      int arg2_count_1=0;
      for(int i=0;i<pipe_arg_count-1;i++)
      {
        if(i==0)
        final_strs(pipe_parsed[i],st_str,&arg2_count_1);
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
      }
      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[pipe_arg_count-1]);i++)
      {
        if(pipe_parsed[pipe_arg_count-1][i]=='>')
        {
          int ki=0;
          for(int j=i+1;j<strlen(pipe_parsed[pipe_arg_count-1]);j++)
          {
            out_file[ki++]=pipe_parsed[pipe_arg_count-1][j];
          }
          out_file[ki]='\0';              //outputfile
          rm_t_space(out_file);
          rm_space(out_file);
          break;
        }
        cmd2[ll++]=pipe_parsed[pipe_arg_count-1][i];
      }
      cmd2[ll]='\0';
      rm_space(cmd2);
      rm_t_space(cmd2);
     if(strlen(out_file)==0)
     {
      fprintf(stderr,"output file not provided\n");
     }
     else
     {
      execute_pipe_n_r(in_cc,out_cc,NULL,cmd2,NULL,out_file,st_str,nd_str,NULL,pipe_arg_count);
     }
    }
    
    if(in_cc==1 && out_cc==2)
    {
      for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* nd_str[100];
      int arg2_count_2=0;
       for(int i=1;i<pipe_arg_count-1;i++)
      {
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
      }
      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[0]);i++)
      {
        if(pipe_parsed[0][i]=='<')
        {
          int ki=0;
          for(int j=i+1;j<strlen(pipe_parsed[0]);j++)
          {
            in_file[ki++]=pipe_parsed[0][j];
          }
          in_file[ki]='\0';           //inputfile
          rm_space(in_file);
          rm_t_space(in_file);
          break;
        }
        cmd1[ll++]=pipe_parsed[0][i];
      }
      cmd1[ll]='\0';
      rm_space(cmd1);
      rm_t_space(cmd1);

      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[pipe_arg_count-1]);i++)
      {
        if(pipe_parsed[pipe_arg_count-1][i]=='>')
        {
          char arraa[3]={pipe_parsed[pipe_arg_count-1][i],pipe_parsed[pipe_arg_count-1][i+1],'\0'};
          if(!strcmp(arraa,">>"))
          {
            int ki=0;
            for(int j=i+2;j<strlen(pipe_parsed[pipe_arg_count-1]);j++)
            {
              out_file[ki++]=pipe_parsed[pipe_arg_count-1][j];
            }
            out_file[ki]='\0';              //outputfile
            rm_t_space(out_file);
            rm_space(out_file);
            break;
          }
        }
        cmd2[ll++]=pipe_parsed[pipe_arg_count-1][i];
      }
      cmd2[ll]='\0';
      rm_space(cmd2);
      rm_t_space(cmd2);

      dlg=0;
      if( access(in_file, F_OK ) != -1 )
      {
        // file exists
      } 
      else
      {
        dlg=1;          // file doesn't exist
      }
      if(!dlg && strlen(out_file)!=0)
      {
        execute_pipe_n_r(in_cc,out_cc,cmd1,cmd2,in_file,out_file,NULL,nd_str,NULL,pipe_arg_count);
      }
      else if(dlg)
      {
        fprintf(stderr,"input file error\n");
      }
      else if(strlen(out_file)==0)
      {
        fprintf(stderr,"output file not provided\n");
      }
      
    }

    if(in_cc==1 && out_cc==1)
    {
      for(int i=0;i<pipe_arg_count;i++)
      {
        rm_space(pipe_parsed[i]);
        rm_t_space(pipe_parsed[i]);
      }
      char* nd_str[100];
      int arg2_count_2=0;
       for(int i=1;i<pipe_arg_count-1;i++)
      {
        if(i==1)
        final_strs(pipe_parsed[i],nd_str,&arg2_count_2);
      }
      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[0]);i++)
      {
        if(pipe_parsed[0][i]=='<')
        {
          int ki=0;
          for(int j=i+1;j<strlen(pipe_parsed[0]);j++)
          {
            in_file[ki++]=pipe_parsed[0][j];
          }
          in_file[ki]='\0';               //inputfile
          rm_space(in_file);
          rm_t_space(in_file);
          break;
        }
        cmd1[ll++]=pipe_parsed[0][i];
      }
      cmd1[ll]='\0';
      rm_space(cmd1);
      rm_t_space(cmd1);

      ll=0;
      for(int  i=0;i<strlen(pipe_parsed[pipe_arg_count-1]);i++)
      {
        if(pipe_parsed[pipe_arg_count-1][i]=='>')
        {
          int ki=0;
          for(int j=i+1;j<strlen(pipe_parsed[pipe_arg_count-1]);j++)
          {
            out_file[ki++]=pipe_parsed[pipe_arg_count-1][j];
          }
          out_file[ki]='\0';            //outputfile
          rm_t_space(out_file);
          rm_space(out_file);
          break;
        }
        cmd2[ll++]=pipe_parsed[pipe_arg_count-1][i];
      }
      cmd2[ll]='\0';
      rm_space(cmd2);
      rm_t_space(cmd2);

      dlg=0;
    
      if( access(in_file, F_OK ) != -1 )
      {
        // file exists
      } 
      else
      {
        dlg=1;            // file doesn't exist
      }
      if(!dlg && strlen(out_file)!=0)
      {
        execute_pipe_n_r(in_cc,out_cc,cmd1,cmd2,in_file,out_file,NULL,nd_str,NULL,pipe_arg_count);
      }
      else if(dlg)
      {
        fprintf(stderr,"input file error\n");
      }
      else if(strlen(out_file)==0)
      {
        fprintf(stderr,"output file not provided\n");
      }
    }
  }


  else
  {
    in_cc=0;
    out_cc=0;
    if(has_input_redirection(input))
    {
      in_cc=1;
    }
    if(has_output_redirection(input))
    {
      out_cc=1;
    }
    if(has_append(input))
    {
      out_cc=2;
    }
    if(in_cc==0 && out_cc==0)
    {
      imp_flag=1;
      parse_input(input,parsed);
    }
    if(in_cc==0 && out_cc==2)
    {
      for(int i=0;i<80;i++)
      {
        out_file[i]='\0';
        in_file[i]='\0';
      }
      ll=0;
      for(int i=0;i<strlen(input);i++)
      {
        if(input[i]=='>')
        {
            char arraa[3]={input[i],input[i+1],'\0'};

            if(!strcmp(arraa,">>"))
            {
                int ki=0;
                for (int j=i+2;j<strlen(input);j++)
                {
                    out_file[ki++]=input[j];
                }
                out_file[ki]='\0';
                rm_space(out_file);
                rm_t_space(out_file);
                break;
            }
            
        }
        cmd1[ll++]=input[i];
      }
      cmd1[ll]='\0';
      rm_t_space(cmd1);
      rm_space(cmd1);
      char* st_str[100];
      int arg2_count_1=0;
      final_strs(cmd1,st_str,&arg2_count_1);
      code_redirect(in_cc,out_cc,NULL,out_file,st_str);
    }
    if(in_cc==1 && out_cc==2)
    {
      for(int  i=0;i<strlen(input);i++)
      {
        if(input[i]=='>')
        {
          char arraa[3]={input[i],input[i+1],'\0'};
          if(!strcmp(arraa,">>"))
          {
            int ki=0;
            for(int j=i+2;j<strlen(input);j++)
            {
              out_file[ki++]=input[j];
            }
            out_file[ki]='\0';
            rm_space(out_file);
            rm_t_space(out_file);
            break;
          }
        }
        cmd2[ll++]=input[i];
      }
      cmd2[ll]='\0';
      ll=0;
      rm_t_space(cmd2);
      rm_space(cmd2);
      char *temp_part=strdup(cmd2);
      for(int  i=0;i<strlen(temp_part);i++)
      {
        if(temp_part[i]=='<')
        {
          int ki=0;
          for(int j=i+1;j<strlen(temp_part);j++)
          {
            in_file[ki++]=temp_part[j];
          }
          in_file[ki]='\0';             //inputfile
          rm_space(in_file);
          rm_t_space(in_file);
          break;
        }
        cmd1[ll++]=temp_part[i];
      }
      cmd1[ll]='\0';
      rm_t_space(cmd1);
      rm_space(cmd1);
      char* st_str[100];
      int arg2_count_1=0;
      dlg=0;
      final_strs(cmd1,st_str,&arg2_count_1);
      if( access(in_file, F_OK ) != -1 )
      {
        // file exists
      } 
      else
      {
        dlg=1;          // file doesn't exist
      } 
      if(dlg!=1)
      {
        code_redirect(in_cc,out_cc,in_file,out_file,st_str);
      }
      else
      {
        fprintf(stderr,"input file error\n");
      }
    }
    if(in_cc==1 && out_cc==0)
    {
      char* i_r_parsed[100];
      char* i_r_parsed2[100];
      for(int i=0;i<100;i++){
        i_r_parsed[i]=NULL;
        i_r_parsed2[i]=NULL;
      }
      i_r_parse(input,i_r_parsed);
      for(int i=0;i<i_r_count;i++)
      {
        rm_space(i_r_parsed[i]);
        rm_t_space(i_r_parsed[i]);
      }
      for(int i=1;i<i_r_count;i++)
      {
        i_r_parse2(i_r_parsed[i],i_r_parsed2);
      }
  
      dlg=0;
      if( access(i_r_parsed2[0], F_OK ) != -1 )
      {
        // file exists
      }
      else
      {
        dlg=1;        // file doesn't exist
      }
      if(!dlg)
      {
      code_redirect(in_cc,out_cc,i_r_parsed2[0],NULL,i_r_parsed);
      }
      else
      {
        fprintf(stderr,"input file error\n");
      }    
    }

    if(out_cc==1 && in_cc==0)
    {
      char* o_r_parsed[100];
      char* o_r_parsed2[100];
      for(int i=0;i<100;i++){
        o_r_parsed[i]=NULL;
        o_r_parsed2[i]=NULL;
      }
      o_r_parse(input,o_r_parsed);
      for(int i=0;i<o_r_count;i++)
      {
        rm_space(o_r_parsed[i]);
        rm_t_space(o_r_parsed[i]);
      }
     
      for(int i=0;i<o_r_count-1;i++)
      {
        o_r_parse2(o_r_parsed[i],o_r_parsed2);
      }
    
     
      code_redirect(in_cc,out_cc,NULL,o_r_parsed[o_r_count-1],o_r_parsed2);
    }

    if(out_cc==1 && in_cc==1)
    {
      char* cmd_io[100];
      for(int  i=0;i<strlen(input);i++)
      {
        if(input[i]=='>')
        {
          int ki=0;
          for(int j=i+1;j<strlen(input);j++)
          {
            out_file[ki++]=input[j];
          }
          out_file[ki]='\0';
          rm_space(out_file);
          rm_t_space(out_file);
          break;
        }
        cmd2[ll++]=input[i];
      }
      cmd2[ll]='\0';
      ll=0;
      rm_t_space(cmd2);
      rm_space(cmd2);
      char *temp_part=strdup(cmd2);
      for(int  i=0;i<strlen(temp_part);i++)
      {
        if(temp_part[i]=='<')
        {
          int ki=0;
          for(int j=i+1;j<strlen(temp_part);j++)
          {
            in_file[ki++]=temp_part[j];
          }
          in_file[ki]='\0';           //inputfile
          rm_space(in_file);
          rm_t_space(in_file);
          break;
        }
        cmd1[ll++]=temp_part[i];
      }
      cmd1[ll]='\0';
      rm_t_space(cmd1);
      rm_space(cmd1);
      for(int i=0;i<100;i++)
      {
        if(i==0)
        {
          cmd_io[i]=strdup(cmd1);         //cmd
          continue;
        }
        cmd_io[i]=NULL;
      }
      dlg=0;
      if( access(in_file, F_OK ) != -1 )
      {
        // file exists
      }
      else
      {
        dlg=1;        // file doesn't exist
      }
      if(!dlg)
      {
      code_redirect(in_cc,out_cc,in_file,out_file,cmd_io);      
      }
      else
      {
        fprintf(stderr,"input file error\n");
      }
      
    }

  }
  char *cmd[100];
  cmd[0]="pwd";
  cmd[1]="quit";
  cmd[2]="cd";
  cmd[3]="echo";
  cmd[4]="ls";
  cmd[5]="pinfo";
  cmd[6]="clear";
  cmd[7]="history";
  cmd[8]="jobs";
  cmd[9]="kjob";
  cmd[10]="bg";
  cmd[11]="cronjob";
  cmd[12]="overkill";
  cmd[13]="fg";
  cmd[14]="setenv";
  cmd[15]="unsetenv";

  for(int i=0;i<MAX;i++)
  {
    if(parsed[i]==NULL)
    {
      break;
    }
    c_count++;
  }

 

  if(imp_flag)
  {

  if(!parsed[0])
  {
    int del=0;
    del++;
  }
  else if(!strcmp(parsed[c_count-1],"&"))
  {
    pid_t pid=fork();
    parsed[c_count-1]=NULL;

    if(pid<0)
    {
      fprintf(stderr,"fork failed\n");
    }
    else if(pid!=0)
    {
      int c_tri=pid+1;
      child_arr[pid_count]=c_tri;
      char ans_arr[500];
      ans_arr[0]='\0';
      
      for(int i=0;i<100;i++)
      {
        if(parsed[i]==NULL)break;
        strcat(ans_arr,parsed[i]);
        strcat(ans_arr," ");
      }
      child_name[pid_count]=strdup(ans_arr);
      pid_count++;
    }
    else
    {
      pid_t pi=fork();

      if(pi<0)
      {
        fprintf(stderr,"fork failed\n");
      }
      else if(pi!=0)
      {
        int s;
        waitpid(pi,&s,0);
        if(WIFEXITED(s))
        {
          printf("%s with pid %d exited normally\n",parsed[0],pi);
        }
        else
        {
          printf("%s with pid %d exited abnormally\n",parsed[0],pi);
        }
        exit(0);
      }
      else
      {
        setpgid(0,0);
        if(execvp(parsed[0],parsed)<0)
        {
          fprintf(stderr,"execvp -- error\n");
        }
        exit(0);
      }
      exit(0);
    }
  }
  else if(!strcmp(parsed[0],cmd[0]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else
    {
      code_pwd();
    }
  }
  else if(!strcmp(parsed[0],cmd[1]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else
    {
      code_exit(history,curr);
    }
  }
  else if(!strcmp(parsed[0],cmd[2]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else
    {
      code_cd(parsed);
    }
  }
  else if(!strcmp(parsed[0],cmd[3]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else
    {
      code_echo(parsed);
    }
  }
  else if(!strcmp(parsed[0],cmd[4]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else{
      code_ls(parsed);
    }
  }
  else if(!strcmp(parsed[0],cmd[5]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else{
      code_pinfo(parsed);
    }
  }
  else if(!strcmp(parsed[0],cmd[6]))
  {
      clear();
  }
  else if(!strcmp(parsed[0],cmd[7]))
  {
    if(!strcmp(parsed[c_count-1],"&"))
    {
      printf("invalid\n");
    }
    else
    {
      if(parsed[1]==NULL)
      {
        code_history(10);
      }
      else
      {
        char s[100];
        int lo=0;
        for(int i=0;i<strlen(parsed[1]);i++)
        {
          s[lo++]=parsed[1][i];
        }
        s[lo]='\0';
        int alo=atoi(s);
        code_history(alo);
      }
    }
  }
  else if(!strcmp(parsed[0],cmd[8]))
  {
    print_jobs();
  }
  else if(!strcmp(parsed[0],cmd[9]))
  {
    if(!parsed[1] || !parsed[2])
    {
      fprintf(stderr,"wrong input\n");
    }
    else
    {
      k_job(parsed[1],parsed[2]);
    }
  }
  else if(!strcmp(parsed[0],cmd[10]))
  {
    if(!parsed[1])
    {
      fprintf(stderr,"wrong input\n");
    }
    else
    {
      bg_job(parsed[1]);
    }
  }
  else if(!strcmp(parsed[0],cmd[13]))
  {
    if(!parsed[1])
    {
      fprintf(stderr,"wrong input\n");
    }
    else
    {
      fg_job(parsed[1]);
    }
  }
  else if(!strcmp(parsed[0],cmd[11]))
  {
    int check_flag=0,store_count=0;
    char *chl_ega[100];
    for(int i=0;i<100;i++)
    {
      chl_ega[i]=NULL;
    }
    if(c_count<7)
    {
      fprintf(stderr,"not enough parameters\n");
    }
    else
    {
      if(strcmp(parsed[1],"-c"))
      {
        fprintf(stderr,"-c not provided\n");
        check_flag=1;
      }
      int up=atoi(parsed[c_count-1]);
      for(int i=2;i<c_count;i++)
      {
        if(!strcmp(parsed[i],"-t"))
        {
          break;
        }
        chl_ega[store_count++]=strdup(parsed[i]);
      }
      if(store_count==c_count-2)
      {
        fprintf(stderr,"-t not provided\n");
        check_flag=1;
      }
      

      if(!check_flag)
      {
        int down=atoi(parsed[store_count+3]);
        int time_check=up/down;
        printf("time_check - %d\n",time_check);
        
        pid_t ppid=fork();
          if(ppid<0)
          {
            fprintf(stderr,"error --forking\n");
          }
          else if(ppid==0)
          {
            pid_t pid=fork();
            if(pid<0)
            {
              fprintf(stderr,"error --forking\n");
            }
            else if(!pid)
            {
              for(int i=0;i<time_check;i++)
              {  
                delay(down*1000);
                pid_t pi=fork();
                if(pi<0)
                {
                  fprintf(stderr,"error --forking\n");
                }
                else if(!pi)
                {
                  if(execvp(chl_ega[0],chl_ega)<0)
                  {
                    fprintf(stderr,"execvp --error\n");
                  }
                  exit(0);
                }
                else
                {
                  wait(NULL);
                }
              }
              exit(0);
            }
            else
            {
              for(int i=0;i<time_check;i++)
              {
                delay(down*1000);
                wait(NULL);
              }
              exit(0);
            }
          }
          else
          {
                //do nothing
          }
        }
      }
    }
  else if(!strcmp(parsed[0],cmd[12]))
  {
    overkill_jobs();
  }
  else if(!strcmp(parsed[0],cmd[14]))
  {
    if(c_count<3)
    {
      fprintf(stderr,"not enough parameters\n");
    }
    else
    {
      int ccou=0;
      char value[100];
      for(int i=1;i<strlen(parsed[2])-1;i++)
      {
        value[i-1]=parsed[2][i];
        ccou++;
      }
      value[ccou]='\0';
      var_set(parsed[1],value);
    }
  }
  else if(!strcmp(parsed[0],cmd[15]))
  {
    if(c_count<2)
    {
      fprintf(stderr,"not enough parameters\n");
    }
    else
    {
      var_unset(parsed[1]);
    }
  }
  else
  {
    int ko=0;
    char ab[100];
    char username[256];
    char a[100];
    getlogin_r(username,sizeof(username));
    for(int i=0;i<MAX;i++){
      if(parsed[i]==NULL)break;
        if(parsed[i][0]=='~'){
          for(int j=1;j<strlen(parsed[i]);j++){
            ab[ko++]=parsed[i][j];
          }
          ab[ko]='\0';          
          na_name(username,a);
          strcat(a,ab);
          parsed[i]=strdup(a);
          ko=0;
          for(int l=0;l<strlen(a);l++){
            a[l]='\0';
          }
        }
      }
    pid_t pid=fork();

    if(pid< -1){
      fprintf(stderr,"error --forking\n");
    }
    else if(pid!=0){
      int jik=0;
      sig_pid=pid;
      for(int i=0;i<500;i++)
      {
        naam[i]='\0';
      }
      for(int i=0;i<strlen(parsed[0]);i++)
      {
        naam[i]=parsed[0][i];
        jik++;
      }
      naam[jik]='\0';
      waitpid(pid,NULL,WUNTRACED);
    }
    else{

      if(execvp(parsed[0],parsed)<0){
        fprintf(stderr,"execvp -- error\n");
      }
      exit(0);
    }
  }
  }
}
  

char* enter(void){
  size_t size=0;
  while(getline(&line,&size,stdin)==-1);
  line[strlen(line)-1]='\0';
  return line;
}
char* hostname(void){
  h_name[999]='\0';
  gethostname(h_name,999);
  return h_name;
} 
void shell(char username[]){
  int count=0,k=1,co=0;
  char arr[200];
  char home[100]="/home/";
  home[7]='\0';
  for(int i=0;i<strlen(username);i++){
    home[6+i]=username[i];
    co++;
  }
  home[co+6]='\0';
  char *dir=pwd();
  if(strlen(dir)<strlen(home)){
    if(strlen(dir)==1){
      arr[0]='/';
      arr[1]='\0';
    }
    else {
      int c=0;
      char na[6]="/home";
      na[6]='\0';
      for(int i=0;i<5;i++){
        arr[i]=na[i];
        c++;
      }
      arr[c]='\0';
    }
}
else{
  arr[0]='~';
for(int i=0;i<strlen(dir);i++){
  if(dir[i]=='/'){
    count++;
  }
    if(count==3){
      for(int j=i;j<strlen(dir);j++){
        arr[k++]=dir[j];
      }
      break;
    }
}
arr[k]='\0';
}
char *h_name=hostname();
printf("%s@%s:%s$ ",username,h_name,arr);
}

int set_flag(char *in,char *ab_arr[100]){
  int cg=0;
  for(int i=0;i<100;i++){
    ab_arr[i]=strsep(&in,";");
    if(ab_arr[i]==NULL)break;
    if(strlen(ab_arr[i])==0){
      i--;
    }
    cg++;
  }
  return cg;
}

int main(int argc, char *argv[], char * envp[])
{
  
  signal(SIGABRT,SIG_IGN);
  signal(SIGINT,handle);
  signal(SIGTSTP,handle);
  for(int i=0;i<1000;i++)
  {
    env_var[i]=NULL;
  }
  for(int i=0; envp[i]!=NULL;i++)
  {
    env_var[i]=strdup(envp[i]);
    count++;
  }
  char *ab_arr[100];
  char username[256];
  char home[100];
  char *parsed[MAX];
  for(int i=0;i<MAX;i++)
  {
    parsed[i]=NULL;
  }
  for(int i=0;i<20;i++){
    history[i]=NULL;
  }
  getlogin_r(username,sizeof(username));
  uname=username;
  na_name(username,home);
  chdir(home);
  int flag=0,cg=0,f_flag=0;
  char a[100];
  char *oo="aloo";
  na_name(username,a);
  char* b="/history.txt";
  strcat(a,b);
  int fd=open(a,O_CREAT | O_RDWR,0600);
  file_read(fd);
  close(fd);
  curr=checko;
  while(1){
    flag=0;
    f_flag=0;
    shell(username);
    int sum,up_count;
    sum=0;
    up_count=0;
    int ghk=0;
    ghk=0;
    char *in=enter();
    for(int i=0;i<strlen(in);i++)
    {
      sum+=(int)in[i];
      if(sum==183 && (i+1)%3==0)
      {
        up_count++;
        sum=0;
      }
    }
    if(up_count)
    {
      shell(username);
      printf("%s\n",history[(abs(((20-up_count)+curr)))%20]);    
      for(int i=0;i<strlen(history[(abs(((20-up_count)+curr)))%20]);i++)
      {
        in[i]=history[(abs(((20-up_count)+curr)))%20][i];
        ghk++;
      }
      in[ghk]='\0';
    }

   if(strcmp(in,oo)==0){
      f_flag=1;
    }
    oo=strdup(in);
    if(!f_flag){
    free(history[curr]);
    history[curr]=strdup(in);
    curr=(curr+1)%20;
    }

    for(int i=0;i<strlen(in);i++){
      if(in[i]==';'){
        flag=1;
        cg=set_flag(in,ab_arr);
        break;
      }
    }
    if(flag){
      for(int i=0;i<cg;i++){ 
        args_exec(ab_arr[i],parsed);
      }
    }
    else{
      args_exec(in,parsed);
    }
  }
  free(line);
  return 0;
}
