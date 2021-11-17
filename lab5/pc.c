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

#define NUMBER 500 /*打出数字总数*/
#define CHILD 5 /*消费者进程数*/
#define BUFSIZE 10 /*缓冲区大小*/

int empty,full,mutux;
int fno;

int main(){
    int i,j,k;
    int datap = 5;
    pid_t p;
    int  buf_out = 0; /*从缓冲区读取位置*/
    int  buf_in = 0; /*写入缓冲区位置*/

    
    if(empty  = sem_open("empty",10) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }
    if(full = sem_open("full",0) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }
    if(mutux = sem_open("mutux",1) < 0 ){
        perror("sem_open() error!\n");
        return -1;
    }

    /*fno = open("buffer.dat",O_CREAT|O_RDWR|O_TRUNC,0666);*/
    fno = open("buffer.dat",O_CREAT|O_RDWR|O_TRUNC);

    /* 将待读取位置存入buffer后,以便 子进程 之间通信 */
    lseek(fno,10*sizeof(int),SEEK_SET);
    write(fno,(char *)&buf_out,sizeof(int));

    /*FILE* file = fopen("out.dat","a");*/

    if( (p = fork()) ==0 ){
        for(i=0;i<NUMBER;i++){
            sem_wait(empty);
            sem_wait(mutux);

            lseek(fno,buf_in*sizeof(int),SEEK_SET);
            write(fno,(char*)&i,sizeof(int));
            buf_in = (buf_in +1) % BUFSIZE;

            sem_post(mutux);
            sem_post(full);
        }
            
        
        return 0;
    }
    else if(p<0) {
        perror("Failed to fork\n");
        return -1;
    }

    for(j=0;j<CHILD;j++){
        if( (p = fork()) ==0 ){
            for(k=0;k<NUMBER/CHILD;k++){
                
                sem_wait(full);
                sem_wait(mutux);

                lseek(fno,10*sizeof(int),SEEK_SET);
                read(fno,(char*)&buf_out,sizeof(int));

                lseek(fno,(buf_out)*sizeof(int),SEEK_SET);
                read(fno,(char*)&datap,sizeof(int));

                buf_out = (buf_out+1)%BUFSIZE;

                lseek(fno,10*sizeof(int),SEEK_SET);
                write(fno,(char*)&buf_out,sizeof(int));

                /*printf("datep: %d\n",datap);*/

                sem_post(mutux);
                sem_post(empty);
                printf("%d    :%d: %d\n",getpid(),datap,buf_out);
                fflush(stdout);
            }
            return 0;
        }
        else if(p<0) {
            perror("Failed to fork\n");
            return -1;
        }
    }

    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutux");

    close(fno);
    /*fclose(file);*/

    wait(0);


    return 0;
}
