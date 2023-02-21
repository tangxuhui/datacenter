#include<string.h>
#include<unistd.h>
//一个现有的进程调用函数fork创建一个新的进程
//子进程和父进程急需执行fork函数后的代码
//fork函数调用一次，返回两次
//子进程返回0，父进程返回子进程的进程ID
//子进程是父进程的副本
//子进程获得了父进程的数据空间、堆和栈的副本，不是共享

int main(){
	int pid = fork();
	if(pid ==0){
		std::cout<<"这是子进程"<<getpid()<<std::endl;
	}
	if(pid !=0){
		std::cout<<"这是父进程"<<getpid()<<std::endl;
	}
}
