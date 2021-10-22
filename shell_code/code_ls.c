#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include "perm_check.h"
#include "pwd.h"

int cc=0;
int y=0;
long long int siz;
char *tim;
long int li;
char *uname;
char *gname;

void code_ls(char** parsed){
  DIR* p;
    char *arr2[100];
    char *arr1[100];
    char arr[100];
    int i=0,j=0;
    struct dirent *d;
    char *qq="-l";
    char *ww="-la";
    char *ee="-a";
    char *rr="-al";
    int kkk=0,flag=0;
    int c_count=0,cou=0;

    if(!parsed[1]){
      char* dir_name=pwd();
      p=opendir(dir_name);
    }
    else if(strcmp(parsed[1],qq) && strcmp(parsed[1],ww) && strcmp(parsed[1],ee) && strcmp(parsed[1],rr))
    {
      fprintf(stderr,"ls --error");
    }
    else if(!strcmp(parsed[1],qq) || !strcmp(parsed[1],ww) || !strcmp(parsed[1],ee) || !strcmp(parsed[1],rr)){
      char* dir_name=pwd();
      p=opendir(dir_name);
    }
    else{
      for(int gb=0;gb<strlen(parsed[1]);gb++){
        if(gb==0 && parsed[1][gb]=='~'){
        c_count++;
        flag=1;
        }
        else{
        arr[kkk++]=parsed[1][gb];
        c_count++;
        }
      } 
    if(flag){
    arr[kkk]='\0';
    char baa_array[300]="/home/amit";
    baa_array[10]='\0';
    for(int mb=0;mb<strlen(arr);mb++){
      baa_array[10+mb]=arr[mb];
      cou++;
    }
    baa_array[10+cou]='\0';
    p=opendir(baa_array);
    }
    else{
      p=opendir(parsed[1]);
    }
    
    }

    while((d=readdir(p))){
    if(d->d_name[0]=='.'){
      arr2[i++]=d->d_name;
    }
    else{
      arr1[j++]=d->d_name;
    }
    }
  
    char *a="-a";
    char *b="-l";
    char *c="-la";
    char *e="-al";
    char arr3[100][100];

    
    
    if(!parsed[1]){
      for(int k=0;k<j;k++){
        printf("%s  ",arr1[k]);
    }
    printf("\n");
    }
    
    else if(!strcmp(parsed[1],a)){
     
        for(int k=0;k<i;k++){
          printf("%s  ",arr2[k]);
        }
        for(int k=0;k<j;k++){
          printf("%s  ",arr1[k]);
        }
        printf("\n");
    }
    else if(!strcmp(parsed[1],b)){
        for(int k=0;k<j;k++){
          perm_check(arr1[k],arr3);
          
          for(int r=0;r<cc;r++){
            printf("%c",arr3[y-1][r]);
          }
          printf("  %3ld",li);
          printf("  %s  %s",uname,gname);
          printf("  %10lld",siz);
          tim[strlen(tim)-1]='\0';
          printf("  %s  ",tim);
          printf("%s\n",arr1[k]);
        }
    }
    else if(strcmp(parsed[1],c)==0 || strcmp(parsed[1],e)==0){
        for(int k=0;k<i;k++){
          perm_check(arr2[k],arr3);
          
          for(int r=0;r<cc;r++){
            printf("%c",arr3[y-1][r]);
          }
          printf("  %3ld",li);
          printf("  %s  %s",uname,gname);
          printf("  %10lld",siz);
          tim[strlen(tim)-1]='\0';
          printf("  %s  ",tim);
          printf("%s\n",arr2[k]);
        }
        for(int k=0;k<j;k++){
          perm_check(arr1[k],arr3);
          
          for(int r=0;r<cc;r++){
            printf("%c",arr3[y-1][r]);
          }
          printf("  %3ld",li);
          printf("  %s  %s",uname,gname);
          printf("  %10lld",siz);
          tim[strlen(tim)-1]='\0';
          printf("  %s  ",tim);
          printf("%s\n",arr1[k]);
        }
    }
    else{
      for(int k=0;k<j;k++){
        printf("%s  ",arr1[k]);
    }
    printf("\n");
    }
}
