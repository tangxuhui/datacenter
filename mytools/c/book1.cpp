#include<_public.h>
#include<iostream>

#define MAXNUMP_ 1000   // 共享的内存数量
#define SHMKEYP_ 0x5095		//共享内存的key 
#define SEMKEYP_ 0x5095         // 信号量的key
// 进程心跳信息的结构体
struct st_pinfo{
	int pid;    // 进程id
	char pname[51];   // 进程名称，可以为空
	int timeout; 	// 超过时间，单位：秒
	time_t atime;	//最后一次心跳的时间，用于整数表示
};

class PActive{
	private:
	 	CSEM m_sem;  //用于给共享内存加锁的信号量id

		int m_shmid;  // 共享内存的id
		int m_pos;     // 当前进程在共享内存进程组中的位置
		struct st_pinfo* m_shm;   // 指向共享内存的地址空间
	public:
		PActive();
		bool AddPInfo(const int timeout,const char* pname); // 把当前进程的心跳信息加入共享内存
		bool UptATime();	// 更新贡献内存中当前进程的心跳时间
		~PActive();
};



PActive::PActive(){
	m_shmid=-1;    // 共享内存的id
	m_pos=-1;	// 当前内存在共享内存进程组中的位置
	m_shm=0;	// 指向共享内存的地址空间
}
bool PActive::AddPInfo(const int timeout,const char* pname){     //将当前进程的心跳信息加入到共享内存中
	if(m_pos!=-1) return true;
	// 创建、获取贡献内存，大小为n*sizeof(struct st_pinfo)
        if((m_shmid=shmget(SHMKEYP_,MAXNUMP_*sizeof(struct st_pinfo),0640|IPC_CREAT))==-1)
        {
                std::cout<<"shmget"<<SHMKEYP_<<" failed"<<std::endl;
                return false;
        }
        if((m_sem.init(SEMKEYP_))==false){
                std::cout<<"m_sem.init"<<SEMKEYP_<<" failed"<<std::endl;
                return false;}

        // 将共享内存连接到当前进程的地址空间
        m_shm=(struct st_pinfo*)shmat(m_shmid,0,0);
        // 创建当前进程心跳信息结构体变量，将本进程的信息填进去
        struct st_pinfo stpinfo;
        memset(&stpinfo,0,sizeof(struct st_pinfo));
        stpinfo.pid=getpid();// 进程id
        STRNCPY(stpinfo.pname,sizeof(stpinfo.pname),pname,50);//进程名称
        stpinfo.timeout=timeout;    // 超时时间，单位：秒
        stpinfo.atime=time(0);  // 最后一次心跳的时间，填当前时间
	
        // 进程id是循环使用的，如果有一个进程异常退出，没有清理自己的心跳信息
        // 它的进程信息就会残留在共享内存中，当前的进程就会重用上述进程的id
        // 这样就会在共享内存中存在两个进程id相同的记录，守护进程检测到残留进程
        // 的心跳时，会向进程id发送退出信号，该信号将五杀当前进程

        // 如果共享内存中存在当前进程编号，一定是其他进程残留的数据，当前进程就重用该位置
        for(int ii=0;ii<MAXNUMP_;ii++){
                if(m_shm[ii].pid==stpinfo.pid){
                        m_pos=ii;break;
                }}

        m_sem.P();
        // 在共享内存中查找一个空位置，把当前进程的心跳信息录入共享内存中
        if(m_pos==-1){

                for(int ii=0;ii<MAXNUMP_;ii++){
                        if(m_shm[ii].pid==0){
                                // 找到一个空位置
                                m_pos=ii;
                                break;
                }
        }}
	if(m_pos==-1){
                m_sem.V();// 解锁
                std::cout<<"共享内存空间已用完"<<std::endl;
                return false;
        }

        memcpy(m_shm+m_pos,&stpinfo,sizeof(struct st_pinfo));
        m_sem.V();//解锁

	return true;
}

bool PActive::UptATime(){// 更新共享内存
	//更新共享内存中本进程的心跳时间
	if(m_pos!=-1) return false;	
	m_shm[m_pos].atime=time(0);
	return true;
}
PActive::~PActive(){
	        // 把当前进程从共享内存中移去
       if(m_pos!=-1) memset(m_shm+m_pos,0,sizeof(struct st_pinfo));
        // 把当前共享内存从当前进程中分离 
       if(m_shm!=0) shmdt(m_shm);
       
}


int main(int argc,char *argv[]){
	if(argc<2){
		std::cout<<"Using:./book procname"<<std::endl;
		return 0;}

	PActive Active;
	Active.AddPInfo(30,argv[1]);
	while(true){
		Active.UptATime();
		sleep(10);
}}
