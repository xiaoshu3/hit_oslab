#include <errno.h>
#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int, iam, const char*, name);

int main(int aggc,char **argv)
{
    if(iam(argv[1]) != -1)
        printf("Input successful\n");
    else printf("error"); 
    return 0;
}
