#include "_public.h"

// 程序退出和信号2、15的处理函数
void EXIT(int sig);

void EXIT(int sig){
	std::cout<<"程序退出，sig="<<sig<<std::endl;
	exit(0);
}

int main(int argc,char* argv[]){
	// 程序的帮助
	if(argc!=4){
		std::cout<<"Using:/project/mytools/bin/gzipfiles pathname matchstr timeout"<<std::endl;
		std::cout<<"Example:/project/mytools/bin/gzipfiles /log/idc \"*.log.20*\" 0.02"<<std::endl;
		std::cout<<"	/project/mytools/bin/gzipfiles /tmp/surfdata \"*.xml,*.json\" 0.01"<<std::endl;
		std::cout<<"	/project/mytools/bin/procctl 300 /project/mytools/bin/gzipfile /tmp/surfdata \"*.xml,*.json\" 0.01"<<std::endl;
		std::cout<<"这是一个工具程序，用于压缩历史的数据文件和日志文件"<<std::endl;
		std::cout<<"将pathname目录以及子目录timeout天之前与matchstr比配的数据压缩,timeout可以是小数"<<std::endl;
		std::cout<<"通过调用/usr/bin/gzip命令压缩文件"<<std::endl;
		return -1;
	}
	// 关闭全部的信号和输入输出
	//CloseIOAndSignal(true);
	signal(SIGINT,EXIT);
	signal(SIGTERM,EXIT);

	//提取文件超时的时间点
	char strTimeOut[21];
	LocalTime(strTimeOut,"yyyy-mm-dd hh24:mi:ss",0-(int)(atof(argv[3])*24*60*60));	
	// 打开目录，CDir.OpenDir()
	CDir Dir;
	if(Dir.OpenDir(argv[1],argv[2],1000,true)==false){
		std::cout<<"Dir.OpenDir("<<argv[1]<<") failed"<<std::endl;
		return -1;
	}
	char strCmd[1024];   // 存放gzip压缩文件的命令
	// 遍历目录的文件名
	while(true){
		// 得到一个文件的信息
		if(Dir.ReadDir()==false) break;
		std::cout<<"FullFileName="<<Dir.m_FullFileName<<std::endl;
		// 与超时的时间点比较，如果更早，就压缩
		if((strcmp(Dir.m_ModifyTime,strTimeOut)<0)&&(MatchStr(Dir.m_FileName,"*.gz")==false))
		{
			// 压缩文件，调用系统gzip命令
			
			SNPRINTF(strCmd,sizeof(strCmd),1000,"/usr/bin/gzip -f %s",Dir.m_FullFileName);
			if(system(strCmd)==0)
				std::cout<<"gzip "<<Dir.m_FullFileName<<" ok"<<std::endl;
			else
				std::cout<<"gzip "<<Dir.m_FullFileName<<" failed"<<std::endl;

	}
		}
	return 0;
}
