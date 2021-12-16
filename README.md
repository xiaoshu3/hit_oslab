# hit_oslab
哈工大操作系统实验 
主要的部分mooc 和 实验指导书讲的很详细了，不过中途遇到了很多的小问题，这里记录一下。

## lab2 系统调用
- 给/include/unistd.h 添加两个宏
这里要注意的一定要修改的是bochs中的此文件，不然会在bochs里的linux0.11编译出错，提示找不到那两个宏。其实实验指导书提到了这点（没注意看...）
- ./run后提示打不开磁盘驱动镜像文件hdc-0.11.img
删除oslab目录下的.lock文件即可。

## lab3 进程运行轨迹的跟踪与统计
- 遇到未定义的问题，貌似include 了几个头文件弄好的，当时没记录，现在记不太清了
- 做实验前最好对fork()和 schedule()函数有个大概的认识，不然实验无从下手，我去看了《linux 0.12内核注释》相关部分再去做的实验，很有帮助。

## lab4 基于内核栈切换的进程切换
- schedule() struct task_struct * pnext = &(init_task.task);需要初始化，不然我试了下页面会闪。
- fork.c 中添加extern long first_return_from_kernel(void); 同理sched.c中添加extern long switch_to(struct task_struct *p, unsigned long address);
- system_call.s 中添加.globl switch_to, first_return_from_kernel
- struct tss_struct *tss = &(init_task.task.tss); 我把这句放到了sched.c 中
- system_call.s 中需要修改 因为在task_struct 中添加 kernelstack 带来的影响。注意到system_call.s 中 这句注释
/# these are offsets into the task-struct. 所以kernelstack 之后的偏移量 + 4 即可。


## lab5 信号量的实现与应用
- 遇到的bug在于父进程的一个信号量empty的value是10，到了子进程value就变为了0...而其他信号量到了子进程value值没变，所以我这里把生产者的调用放到了父进程的最后，
把结果打印到文件里要好很多。
- 还有的地方就是pc.c文件中 注意是BUFSIZE，stdio.h 里还定义了个BUFSIZ，``#deffine BUFSIZ 8096``
一开始vscode里就跳出这个，这里看了几遍才发现过来...

## lab6 地址映射也共享
- 还是遇到那个问题，父进程的信号量的value到了子进程就变成了0..所以就一个生产者，一个消费者，把这题过了。
- 之前想了半天怎么去找一个进程没用到而且刚好是处在4M边缘的线性地址空间，主要还是在于怎么判断未使用吧...然后去看了答案.
