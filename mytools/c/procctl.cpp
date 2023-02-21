#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(int argc,char* argv[])
{
	if(argc<3){
		std::cout<<"Using:./procctl timetvl program argv ..."<<std::endl;
		std::cout<<"Example:/home/datacenter/Meteorological-data-center-project/mytools/c/procctl 4 /usr/bin/ls -lt"<<std::endl;
		std::cout<<"本程序是服务程序的调度程序，周期性启动服务程序或shell脚本"<<std::endl;
		std::cout<<"timetvl 运行周期，单位秒，被调度的程序运行结束后，在timetvl秒后被procctl重新启动"<<std::endl;
		std::cout<<"program 被调度的程序名，必须使用全路径"<<std::endl;
		std::cout<<"argvs 被调度的程序的参数"<<std::endl;
		std::cout<<"本程序不会被kill杀死，但可以用kill -9"<<std::endl;
	}
	// 先执行fork函数，创建一个子进程，让子进程调用execl执行新的程序
	// 新程序将替换子程序，不会影响父进程
	// 在父进程中，可以调用wait函数等待新程序运行的结果，这样实现调度功能
	for(int ii=0;ii<64;ii++){
		signal(ii,SIG_IGN); // 关闭信号
		close(ii);     //关闭IO。本程序不希望被打扰
	}

	// 生成子进程，父进程退出，让程序运行在后台，由系统1号进程托管
	if(fork()!=0) exit(0);
	
	// 启动SIGCHLD信号，让父进程可以wait子进程退出的状态
	signal(SIGCHLD,SIG_DFL);

	char* pargv[argc];
	for(int ii=2;ii<argc;ii++){
		pargv[ii-2]=argv[ii];}
	pargv[argc-2]=NULL;
	while(true){
		if(fork()==0){
			execv(argv[2],pargv);
			exit(0);
		}
		else{
			int status;
			wait(&status);
			sleep(atoi(argv[1]));
		}}
	return 0;
}
