//生成气象站点观测的分钟数据


#include "_public.h"
CLogFile logfile;
int main(int argc, char *argv[]){
	//inifile outpath logfile
	if (argc!=4)
	{
		std::cout<<"Using:./crtsurfdata1 inifile outpath logfile"<<std::endl;
		std::cout<<"Example:/home/project/myidc/bin/crturfdata1 /home/project/myidc/ini/stcode.ini /tmp/surfdata /log/idc/crturfdata1.log"<<std::endl;
		std::cout<<"inifile 全国气象站点参数文件名"<<std::endl;
		std::cout<<"outpath 全国气象战点数据文件存放的目录"<<std::endl;
		std::cout<<"logfile 日志文件"<<std::endl;
		return -1;
	}
	if (logfile.Open(argv[3])==false){
		std::cout<<"logfile.Open("<<argv[3]<<") failed"<<std::endl;
		return -1;
	}	
	else{
		logfile.Write("crtsurfdata1 开始运行！");
		// 业务代码
		logfile.Write("crtsurfdata1 运行结束！");
	}

	return 0;
}
