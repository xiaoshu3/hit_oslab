#define __LIBRARY__  
#include <unistd.h>  
#include <linux/sched.h>  
#include <linux/kernel.h>  
#include <asm/segment.h>  
#include <asm/system.h>

#define SEM_COUNT 32
#define QUE_LEN 16
#define SEM_FAILED  (void*) 0

/*队列相关操作，rear始终是下一个待写入的位置，front始终是队列第一个元素*/
struct semaphore_queue
{
	int front;
	int rear;
	struct task_struct *wait_tasks[QUE_LEN];
};
typedef struct semaphore_queue sem_queue;

typedef struct{
    char name[20];
    int value;
    int occupied;
    sem_queue wait_queue;
}semtable[SEM_COUNT];

semtable semaphores;


void init_queue(sem_queue* q){
    q->front  = q->rear =0;
}

int is_full(sem_queue* q){
    return (q->rear + 1) % QUE_LEN  == q->front ? 1:0;
}

int is_empty(sem_queue* q){
    return q->rear == q->front ? 1: 0;
}

struct task_struct* get_task(sem_queue* q){
    if(is_empty(q)){
        printk("queue is empty!!!");
        return NULL;
    }
    struct task_struct * tmp = q->wait_tasks[q->front];
    q->front = (q->front + 1) % QUE_LEN;
    return tmp;
}

int insert_queue(struct task_struct* tmp,sem_queue* q){
    if(is_full(q)){
        printk("queue is full!!!");
        return -1;
    }
    q->wait_tasks[q->rear] = tmp;
    q->rear = (q->rear + 1 )% QUE_LEN;
    return 1; 
}

int sys_sem_open(const char*name,unsigned int value){
    char tmp[20];
    char c;
	int i;
	for( i = 0; i<20; i++)
	{
		c = get_fs_byte(name+i);
		tmp[i] = c;
		if(c =='\0') break;
	}
    /*printk("sem_open\n");*/
    for(i=0;i<SEM_COUNT;i++){
        if(semaphores[i].occupied == 1 ){
            if(strcmp(semaphores[i].name,tmp) == 0){
                printk("Find it\n");
                return i;
            }
        }
    }
    for(i = 0;i<SEM_COUNT;i++){
        if(semaphores[i].occupied == 0){
            strcmp(semaphores[i].name,tmp);
            semaphores[i].occupied =1;
            semaphores[i].value =value;
            init_queue(&(semaphores[i].wait_queue));
            printk("Creat it\n");
            return i;
        }
    }

    printk("Numbers of semaphores are limited!\n");
    return -1;
}

int sys_sem_wait(int sem){
    /*printk("sem_wait\n");*/
    cli();
    --semaphores[sem].value;
    if(semaphores[sem].value <0){
        insert_queue(current,&(semaphores[sem].wait_queue));
        current->state = TASK_UNINTERRUPTIBLE;
	    schedule();
    }
    sti();
    return 0;
}

int sys_sem_post(int sem){
    /*printk("sem_post\n");*/

    cli();
    ++semaphores[sem].value;
    if(semaphores[sem].value <= 0){
        struct task_struct* tmp = get_task(&(semaphores[sem].wait_queue));
        if(tmp){
            (*tmp).state = TASK_RUNNING;
        } 
    }
    sti();
    return 0;
}

int sys_sem_unlink(const char* name){
    /*printk("sem_unlink\n");*/
    char tmp[20];
    char c;
	int i;
	for( i = 0; i<20; i++)
	{
		c = get_fs_byte(name+i);
		tmp[i] = c;
		if(c =='\0') break;
	}
	if(c >= 20) 
	{
		printk("Semphore name is too long!");
		return -1;
	}

    for(i=0;i<SEM_COUNT;i++){
        if(semaphores[i].occupied ==1 && strcmp(semaphores[i].name,tmp) ==0){
            semaphores[i].occupied = 0;
            semaphores[i].value = 0;
            strcmp(semaphores[i].name,'\0');
            return 0;
        }
    }
    return -1;
}