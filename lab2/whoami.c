#include <errno.h>
#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall2(int, whoami,char*,name,unsigned
int,size);

int main()
{   
    char s[24];
    if(whoami(s,24) != -1)
        printf("%s\n",s);    
    return 0;
}

