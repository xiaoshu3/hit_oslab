#define __LIBRARY__ 
#include <unistd.h>  
#include <linux/sched.h>  
#include <linux/kernel.h>  
#include <errno.h>
#include <asm/segment.h>  
#include <asm/system.h>

#define SHM_MAX_SIZE 20

struct struct_shm_tables{
    int occupied;
    int shm_id;
    long address;
}shm_tables[SHM_MAX_SIZE];

/*
int find_shm_location(int shm_id){
    int i;
    for(i=0;i<SHM_MAX_SIZE;i++){
        if(shm_tables[i].shm_id == shm_id && shm_tables[i].occupied == 1)
            return i;
    }
    return -1;
}
*/


int sys_shmget(int key, size_t size, int shmflg){
    if(size > 4096) return -(EINVAL);
    int i;
    long tmp;

    for(i=0;i<SHM_MAX_SIZE;i++){
        if(shm_tables[i].occupied==1 && shm_tables[i].shm_id == key){
            return i;
        }
    }
    for(i=0;i<SHM_MAX_SIZE;i++){
        if(!(shm_tables[i].occupied)){
            if(!(tmp = get_free_page()))
                return -(ENOMEM);
        }
        shm_tables[i].address = tmp;
        shm_tables[i].occupied = 1;
        shm_tables[i].shm_id = key;
        return i;
    }
    return -(ENOMEM);   
}

void *sys_shmat(int shmid, const void *shmaddr, int shmflg){
    if(shm_tables[shmid].occupied == 0)
        return -(EINVAL);

    if(!(put_page(shm_tables[shmid].address,current->brk + current->start_code)))
        return -(ENOMEM);
    /*
    return (void*)((current->brk) & ~4095);
    */
   return (void*)current->brk;

}