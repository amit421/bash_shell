#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>

void code_cd(char** parsed)
{
  char directory[100];
  int k=0;
  int flag=0;
  int sp_flag=0;
  int mor_flag=0;
  if(!parsed[1])
  {
    sp_flag=1;
  }
  if(!sp_flag)
  {
    
      for(int i=0;i<strlen(parsed[1]);i++)
      {
        char *tilda="~";
        if(i==0 && parsed[1][i]=='~')
        {
          if(!strcmp(parsed[1],tilda))
          {
            flag=1;
            break;
          }
          else
          {
            for(int j=1;j<strlen(parsed[1]);j++)
            {
              directory[k++]=parsed[1][j];
            }
            directory[k]='\0';
            flag=2;
            break;
          }
        }
        else if(parsed[1][i]=='~')
        {
          fprintf(stderr,"invalid");
        }
      }
      if(flag==1)
      {
        int count=0;
        char username[100];
        getlogin_r(username,sizeof(username));
        char home[100]="/home/";
        home[7]='\0';
        for(int i=0;i<strlen(username);i++)
        {
          home[6+i]=username[i];
          count++;
        }
        home[count+6]='\0';
        DIR* dir = opendir(home);
        if (dir)
        {
          /* Directory exists. */
          closedir(dir);
        }
        else if (ENOENT == errno) 
        {
          mor_flag=1;
          if(mor_flag==1)
          {
            fprintf(stderr,"cd error --directory doesn't exist\n");
          }
        }
        if(!mor_flag)
        {
          chdir(home);
        }
      }
      else if(flag==2)
      {
        int count=0;
        char username[100];
        getlogin_r(username,sizeof(username));
        char home[100]="/home/";
        home[7]='\0';
        for(int i=0;i<strlen(username);i++)
        {
          home[6+i]=username[i];
          count++;
        }
        int co=count;
        home[count+6]='\0';
        for(int i=0;i<strlen(directory);i++)
        { 
          home[co+6+i]=directory[i];
          count++;
        }
        home[count+6]='\0';
        DIR* dir = opendir(home);
        if (dir) 
        {
          /* Directory exists. */
          closedir(dir);
        } 
        else if (ENOENT == errno) 
        {
          mor_flag=1;
          if(mor_flag==1)
          {
            fprintf(stderr,"cd error --directory doesn't exist\n");
          }
        }
        if(!mor_flag)
        {
        chdir(home);
        }
      }
      else
      {
        DIR* dir = opendir(parsed[1]);
        if (dir) 
        {
          /* Directory exists. */
          closedir(dir);
        } 
        else if (ENOENT == errno) 
        {
          mor_flag=1;
          if(!strcmp(parsed[1],".."))
          {
            mor_flag=0;
          }
          if(mor_flag==1)
          {
            fprintf(stderr,"cd error --directory doesn't exist\n");
          }
        }
        if(!mor_flag)
        {
          chdir(parsed[1]);
        }
      }
    }
  else
  { 
    fprintf(stderr,"cd error -- no arguments to cd\n");
  }
}