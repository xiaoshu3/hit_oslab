
#include <string.h>
#include <errno.h>
#include <asm/segment.h>

char uname[24];

int sys_iam(const char* name)
{
    //printk("hello,world\n");
    int len =0;
    while(get_fs_byte(name + len) != '\0') len++;
    if(len > 23){
        errno = EINVAL;
        return -(EINVAL);
    }
    int i;
    for(i=0;i<=len;i++) 
        *(uname + i) = get_fs_byte(name + i);
        
    //*(uname + i) = '\0';
    return len;
}

int sys_whoami(char *name,unsigned int size)
{
    //printk("end happy\n");
    int len =0;
    while(*(uname +len)!= '\0') len++;
    if(len >= size){
        errno = EINVAL;
        return -(EINVAL);
    }
    int i;
    for(i=0;i<=len;i++){
        put_fs_byte(uname[i],name+i);
    }
    return len;
}

