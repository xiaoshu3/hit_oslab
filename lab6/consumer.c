#define   __LIBRARY__
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

_syscall2(int,sem_open,const char *,name,unsigned int,value);
_syscall1(int,sem_wait,int,sem);
_syscall1(int,sem_post,int,sem);
_syscall1(int,sem_unlink,const char *,name);

_syscall3(int,shmget,int,key,size_t,size,int,shmflg);
_syscall3(void*,shmat,int,shmid,const void*,shmaddr,int,shmflg);

#define NUMBER 500 /*打出数字总数*/
#define BUFSIZE 10 /*缓冲区大小*/

int empty,full,mutux;
int data[BUFSIZE + 1];

int main(){
    int  buf_in = 0; /*写入缓冲区位置*/
    int i;
    int shmid;
    int* shared;
    void* shm  =NULL;
    if((shmid = shmget(123,sizeof(data),0)) < 0 ){
        perror("shmget failed\n");
        return -1;
    }
    if((shm = shmat(shmid,0,0)) == (void*)-1){
        perror("shmat failed\n");
        return -1;
    }

    printf("\nMemory attached at %X\n", (int)shm);
    
    shared = (int*)shm;

    if((empty  = sem_open("empty",10)) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }
    if((full = sem_open("full",0)) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }
    if((mutux = sem_open("mutux",1)) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }

    for(i=0;i<NUMBER;i++){
        sem_wait(empty);
        sem_wait(mutux);

        /*
        printf("enter!!!\n");
        */
        shared[buf_in] = i;
        buf_in = (buf_in +1) % BUFSIZE;

        sem_post(mutux);
        sem_post(full);
    }

    wait(&i);

    printf("consumer end\n");
    return 0;
    
}