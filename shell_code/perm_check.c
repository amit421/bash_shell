#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include<grp.h>
#include "code_ls.h"

long long int siz;
char *tim;
long int li;
char *uname;
char *gname;

void perm_check(char *f,char arr[100][100]){
  cc=0;
  
  struct stat sd;
  
  int val1=stat(f,&sd);

  if(val1<0){
    perror("error");
  }
  else{
    if(S_ISREG(sd.st_mode)){
      arr[y][cc++]='-';
    }
    else
    {
      arr[y][cc++]='d';
    }
    
    if(sd.st_mode & S_IRUSR){
			arr[y][cc++]='r';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IWUSR){
			arr[y][cc++]='w';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IXUSR){
			arr[y][cc++]='x';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IRGRP){
			arr[y][cc++]='r';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IWGRP){
			arr[y][cc++]='w';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IXGRP){
			arr[y][cc++]='x';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IROTH){
			arr[y][cc++]='r';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IWOTH){
			arr[y][cc++]='w';
		}
		else{
			arr[y][cc++]='-';
		}
		if(sd.st_mode & S_IXOTH){
			arr[y][cc++]='x';
		}
		else{
			arr[y][cc++]='-';
		}
    siz=sd.st_size;
    tim=ctime(&sd.st_mtime);
    li=sd.st_nlink;
  }
  struct group *grp=NULL;
  struct group gr;
  char buffer[1000];
  getgrnam_r(uname,&gr,buffer,1000,&grp);
  gname=grp->gr_name;
  y++;
}