#include<iostream>
#include "_public.h"

CSEM sem;    // 用于给共享内存加锁的信号量

struct st_pid{
	int pid;   //进程编号
	char name[51];		   //进程名称
};

int main(int argc,char* argv[]){   //ipcs -m 显示当前系统的共享内存 ipcrm -m id  删除共享内存
	//共享内存的标志
	int shmid;

	// 获取或者创建共享内存，键值为0x5005
	if((shmid=shmget(0x5005,sizeof(struct st_pid),0640|IPC_CREAT))==-1){
		std::cout<<"shmget(0x5005) failed"<<std::endl;}

	// 如果信号量已存在，获取信号量；如果信号量不存在，则创建它并初始化为value
	if(sem.init(0x5005)==false){
		std::cout<<"sem.init(0x5005) failed;"<<std::endl;
		return -1;}

	// 用于指向共享内存的结构体变量
	struct st_pid* stpid=0;

	// 把共享内存连接到当前进程的地址空间
	if((stpid=(struct st_pid*)(shmat(shmid,0,0)))==(void*)-1){
		std::cout<<"shmat failed"<<std::endl;
		return -1;
	}


	std::cout<<"aaa time="<<time(0)<<",val="<<sem.value()<<std::endl;
	sem.P();    //加锁
	std::cout<<"bbb time="<<time(0)<<",val="<<sem.value()<<std::endl;

	std::cout<<"pid="<<stpid->pid<<",name="<<stpid->name<<std::endl;
	stpid->pid=getpid();       // 进程编号
	sleep(10);
	strcpy(stpid->name,argv[1]);	// 进程名称
	std::cout<<"ccc time="<<time(0)<<",val="<<sem.value()<<std::endl;

	std::cout<<"pid="<<stpid->pid<<",name="<<stpid->name<<std::endl;
	sem.V();    //解锁
	std::cout<<"ddd time="<<time(0)<<",val="<<sem.value()<<std::endl;

		    

	// 把共享内存从当前进程中分离
	shmdt(stpid);

	//删除共享内存
	if(shmctl(shmid,IPC_RMID,0)==-1){
		std::cout<<"shmctl failed"<<std::endl;
		return -1;
	}
	return 0;
}
