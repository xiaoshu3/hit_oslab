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
#define CHILD 5 /*消费者进程数*/

int empty,full,mutux,outfile;


int main(){
    int i,j,tmp;
    pid_t p;
    int shmid;
    int* shared;
    void* shm  =NULL;
    int  buf_out = 0; /*从缓冲区读取位置*/
    FILE * fp;

    /*
    printf("empty:%d  full:%d  mutux:%d\n",empty,full,mutux);
    */
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
    if((outfile = sem_open("outfile",1)) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }

    printf("prod:%d %d %d %d\n",empty,full,mutux,outfile);


    if((shmid = shmget(123,0,0)) < 0 ){
        perror("shmget failed\n");
        return -1;
    }
    if((shm = shmat(shmid,0,0)) == (void*)-1){
        perror("shmat failed\n");
        return -1;
    }

    printf("\nMemory attached at %X\n", (int)shm);

    shared = (int*)shm;

    /*fno = open("buffer.dat",O_CREAT|O_RDWR|O_TRUNC,0666);*/
    fp = fopen("shm.txt","w");

    /*
    for(i=0;i<CHILD;i++){
        if((p = fork()) == 0){
            for(j=0;j< NUMBER/CHILD;j++){
                sem_wait(full);
                sem_wait(mutux);

                buf_out = shared[BUFSIZE];
                tmp = shared[buf_out];
                buf_out = (buf_out+1)%BUFSIZE;
                shared[BUFSIZE] = buf_out;
                /*printf("datep: %d\n",datap);/

                sem_post(mutux);
                sem_post(empty);
                /*
                printf("%d    :%d: %d\n",getpid(),datap,buf_out);
                fflush(stdout);
                /
               sem_wait(outfile);
               fprintf(fp,"%d    :%d: %d\n",getpid(),tmp,buf_out);
               fflush(fp);
               sem_post(outfile);
            }
            return 0;
        }
    }
    */
    for(j=0;j<NUMBER;j++){
        sem_wait(full);
        sem_wait(mutux);
        buf_out = shared[BUFSIZE];
        tmp = shared[buf_out];
        buf_out = (buf_out+1)%BUFSIZE;
        shared[BUFSIZE] = buf_out;

        sem_post(mutux);
        sem_post(empty);
                
        sem_wait(outfile);
        fprintf(fp,"%d    :%d: %d\n",getpid(),tmp,buf_out);
        fflush(fp);
        sem_post(outfile);
    }

    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutux");
    sem_unlink("outfile");

    /*fclose(file);*/
    fclose(fp);

    wait(&p);

    sleep(20);
    printf("producer return\n");
    
    return 0;
}
