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

