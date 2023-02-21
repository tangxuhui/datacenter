//生成气象站点观测的分钟数据


#include "_public.h"
//全国气象站点参数结构体
struct st_stcode{
	char provname[31];
	char obtid[31];
	char obtname[31];
	double lat;
	double lon;
	double height;
};

//存放全国气象站点参数的容器
vector<struct st_stcode> vstcode;

//把站点参数文件加载到vstcode容器中
bool LoadSTCode(const char* inifile);

CLogFile logfile;

//实现将站点参数文件加载到vstcode容器中
bool LoadSTCode(const char* inifile){
	CFile File;
	//打开站点参数文件
	if(File.Open(inifile,"r")==false){
	logfile.Write("File.Open(%s) failed.\n",inifile);
	return false;
	}
	
	//存放每一行读出的数据
	char strBuffer[301];
	//用于拆分字符串
	CCmdStr CmdStr;
	//结构体用于存储strbuffer中的信息
	struct st_stcode stcode;
	while(true){
		//从站点参数文件中读取一行，如果已读取完，跳出循环

		if(File.Fgets(strBuffer,300,true)==false) break;
				
		
		//把读取到的一行拆分
		CmdStr.SplitToCmd(strBuffer,",",true);
		
		//去掉第一行（无效数据）
		if(CmdStr.CmdCount()!=6) continue;
		
		//把站点参数的每个数据保存在站点参数结构体中
		CmdStr.GetValue(0, stcode.provname,30); //省
		CmdStr.GetValue(1, stcode.obtid,10);
		CmdStr.GetValue(2, stcode.obtname,30);
		CmdStr.GetValue(3, &stcode.lat);
		CmdStr.GetValue(4, &stcode.lon);
		CmdStr.GetValue(5, &stcode.height);
		
		//把站点参数结构体放在站点参数容器
		vstcode.push_back(stcode);
	}

	//关闭文件
	for(int ii=0;ii<vstcode.size();ii++){
		logfile.Write("provname=%s,obtid=%s,obtname=%s,lat=%.2f,lon=%.2f,height=%.2f\n",\
				vstcode[ii].provname,vstcode[ii].obtid,vstcode[ii].obtname,vstcode[ii].lat,\
				vstcode[ii].lon,vstcode[ii].height);
	}
	return true;
}




int main(int argc, char *argv[]){
	//inifile outpath logfile
	if (argc!=4)
	{
		std::cout<<"Using:./crtsurfdata2 inifile outpath logfile"<<std::endl;
		std::cout<<"Example:/home/project/myidc/bin/crturfdata2 /home/project/myidc/ini/stcode.ini /tmp/surfdata /log/idc/crturfdata1.log"<<std::endl;
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
		logfile.Write("crtsurfdata2 开始运行！");
		//把站点参数文件加载到vstcode容器中
		if(LoadSTCode(argv[1])==false) return -1; 
		logfile.Write("crtsurfdata2 运行结束！");
	}

	return 0;
}
