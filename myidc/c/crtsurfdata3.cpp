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

// 全国气象站点分钟观测数据结构
struct st_surfdata{
	char obtid[11];       // 站点代码
	char ddatatime[21];   // 数据时间：格式yyyymmddhh24miss
       	int t;		      // 气温：单位 0.1摄氏度
 	int p;	 	      // 气压：0.1百帕
	int u;		      // 相对湿度，0-100之间的值
	int wd;		      // 风向：0-360之间的值
	int wf;		      // 风速：单位0.1m/s
	int r;			// 降雨量：0.1mm
	int vis;		// 能见度：0.1米
};	

vector<struct st_surfdata> vsurfdata; // 存放全国气象站点分钟观测数据的容器

// 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中
void CrtSurfData();




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

// 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中
void CrtSurfData(){
	// 播随机数种子
	srand(time(0));

	// 获取当前时间，生成观测时间
	char strddatetime[21];
	memset(strddatetime,0,sizeof(strddatetime));
	LocalTime(strddatetime,"yyyymmddhh24miss");
	struct st_surfdata stsurfdata;
	// 遍历气象站点参数的vscode容器
	for(int ii=0;ii<vstcode.size();ii++){
		memset(&stsurfdata,0,sizeof(struct st_surfdata));
		// 用随机数填充分钟观测数据的结构体
		strncpy(stsurfdata.obtid,vstcode[ii].obtid,10);
		strncpy(stsurfdata.ddatatime,strddatetime,14);	//时间 格式yyyymmddhh24miss
		stsurfdata.t=rand()%351;	//气温，0.1摄氏度
		stsurfdata.p=rand()%265+10000;	//气压0.1百帕
		stsurfdata.u=rand()%100+1;	//相对湿度，0-100
		stsurfdata.wd=rand()%360;	//风向0-360
		stsurfdata.wf=rand()%150;	//风速:0.1m/s
		stsurfdata.r=rand()%16;		//降雨量：0.1mm
		stsurfdata.vis=rand()%5001+100000; //能见度：0.1m
		// 把观测数据的结构体放入vsurfdata容器
		vsurfdata.push_back(stsurfdata);
	}
	// 验证代码
	printf("success\n");
}


int main(int argc, char *argv[]){
	//inifile outpath logfile
	if (argc!=4)
	{
		std::cout<<"Using:./crtsurfdata3 inifile outpath logfile"<<std::endl;
		std::cout<<"Example:/home/project/myidc/bin/crturfdata3 /home/project/myidc/ini/stcode.ini /tmp/surfdata /log/idc/crturfdata3.log"<<std::endl;
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
		logfile.Write("crtsurfdata3 开始运行！");
		//把站点参数文件加载到vstcode容器中
		if(LoadSTCode(argv[1])==false) return -1;
	        // 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中
		CrtSurfData();	
		logfile.Write("crtsurfdata3 运行结束！");
	}

	return 0;
}
