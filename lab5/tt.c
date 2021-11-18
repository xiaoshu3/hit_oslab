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

int main(){
    int d,b;
    
    d =sem_open("hello",2);
    b = sem_open("hello",3);
    printf("b: %d\n",b);
    printf("d: %d\n",d);

    return 0;

}
