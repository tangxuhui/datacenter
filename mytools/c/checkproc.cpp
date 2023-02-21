#include<iostream>
#include"_public.h"

// 程序运行的日志
CLogFile logfile;

int main(int argc,char* argv[]){
	// 程序的帮助
	if(argc!=2){
		std::cout<<"Using:./checkproc logfilename"<<std::endl;
		std::cout<<"Example:/project/mytools/bin/procctl 10 /project/mytools/bin/checkproc /tmp/log/checkproc.log"<<std::endl;
		std::cout<<"本程序用于检测后台服务程序是否超时，如果超时就终止"<<std::endl;
		std::cout<<"本程序由procctl启动，运行周期十秒"<<std::endl;
		std::cout<<"为了避免普通用户误杀，程序用root启动"<<std::endl;
		return 0;
	}
	// 忽略全部信号，不希望程序被打扰
	//for(int ii=1;ii<=64;ii++) signal(ii,SIG_IGN); 等同于下
	CloseIOAndSignal(true);

	//  打开日志文件
	if(logfile.Open(argv[1],"a+")==false){
		std::cout<<"logfile.Open() failed"<<std::endl;
	}
	//  创建、获取共享内存,键值为SHMKEYP，大小为MAXNUMP个st_procinfo
	int shmid=0;
	if((shmid=shmget(SHMKEYP,MAXNUMP*sizeof(struct st_procinfo),0666|IPC_CREAT))==-1){
		logfile.Write("创建、获取内存(%x)失败\n",SHMKEYP);
		return false;
	}
	//  将共享内存连接到当前进程的地址空间
	struct st_procinfo* shm = (struct st_procinfo*)shmat(shmid,0,0);
	//  遍历共享内存中的全部记录
	for(int ii=0;ii<MAXNUMP;ii++){
		// 如果记录的pid==0,表示空记录，continue
		if(shm[ii].pid==0) continue;
		// 如果记录的pid!=0,表示服务程序的心跳记录
		logfile.Write("ii=%d,pid=%d,pname=%s,timeout=%d,atime=%d\n",\
				ii,shm[ii].pid,shm[ii].pname,shm[ii].timeout,shm[ii].atime);
		// 向进程发送信号0，判断是否存在，如果不存在，删除记录 continue
		int iret=kill(shm[ii].pid,0);
		if(iret==-1){
			logfile.Write("进程pid=%d(%s)不存在\n",(shm+ii)->pid,(shm+ii)->pname);
			memset(shm+ii,0,sizeof(struct st_procinfo));//从共享内存中删除该记录
			continue;}	
		// 如果进程未超时 continue
		time_t now = time(0); //获取当前时间
		if(now-shm[ii].atime<shm[ii].timeout) continue;
		// 如果超时
		logfile.Write("进程pid=%d(%s)已经超时\n",(shm+ii)->pid,(shm+ii)->pname);
		// 发送信号15，尝试正常终止进程
		kill(shm[ii].pid,15);

		//服务程序退出需要时间，每个1s进行一次判断
		for(int jj=0;jj<5;jj++){
			sleep(1);
			iret = kill(shm[ii].pid,0);
			if(iret==-1) //若进程不存在
				break;
		}
		// 如果进程仍存在，发送信号9，强制终止
		if(iret==-1)
			logfile.Write("进程pid=%d(%s)已经正常终止\n",(shm+ii)->pid,(shm+ii)->pname);
		else{
			kill(shm[ii].pid,9); // 如果进程仍存在，发送9强制终止
			logfile.Write("进程pid=%d(%s)已经正常终止\n",(shm+ii)->pid,(shm+ii)->pname);
		}

		
		// 删除已超时的进程的心跳记录
		memset(shm+ii,0,sizeof(struct st_procinfo)); // 从共享内存中删除该记录
		}
	
	// 把共享内存从当前进程分离
	shmdt(shm);
	
	return 0;
}
