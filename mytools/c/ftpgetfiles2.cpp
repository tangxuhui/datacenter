#include "_ftp.h"
#include<iostream>
CLogFile logfile;
Cftp ftp;
struct st_arg{
	char host[31];
	int mode;
	char username[31];
	char password[31];
	char remotepath[301];	
	char localpath[301];
	char matchname[101];
	char listfilename[301]; // 下载前列出服务器文件名的文件
}starg;
// 程序退出和信号2，15处理函数
void EXIT(int sig);
void EXIT(int sig){
	std::cout<<"程序退出，sig="<<sig<<std::endl;
	exit(0);
}
void _help();
// 把xml解析到参数starg结构中
bool _xml2arg(char *strxmlbuffer);
// 下载文件功能的主函数
bool _ftpgetfiles();

int main(int argc,char* argv[]){    // 将ftp服务上的某文件下载到本地目录
	// 把服务器上某目录的文件全部下载到本地目录
	// 日志文件名 ftp服务器的ip和端口 传输模式（主动或被动） 用户名 密码
	// 服务器存放文件的目录 本地存放文件的目录 下载文件名匹配的规则
	if(argc!=3){
	_help();
	return -1;	
	}
	// 关闭全部的信号和输入输出
	// CloseIOAndSignal();
	signal(SIGINT,EXIT);
	signal(SIGTERM,EXIT);

	// 打开日志文件
	if(logfile.Open(argv[1],"a+")==false){
		std::cout<<"打开日志文件失败("<<argv[1]<<")"<<std::endl;
	}
	// 解析xml，得到程序运行的参数
	if(_xml2arg(argv[2])==false) return -1;
	// 登录ftp服务器
	if(ftp.login(starg.host,starg.username,starg.password,starg.mode)==false){
		logfile.Write("ftp.login(%s,%s,%s) failed.\n",starg.host,starg.username,starg.password);
		return -1;
	}

	if(_ftpgetfiles()==false){
		logfile.Write("function _ftpgetfiles failed.\n");
	}

	ftp.logout();
	return 0;
}

void _help(){
        std::cout<<"Using:/datacenter/mytools/bin/ftpgetfiles1 logfilename xmlbuffer"<<std::endl;
        std::cout<<"Sample:./datacenter/mytools/bin/procctl 30 ./ftpgetfiles /log/ftpgetfiles_surfdata.log \"<host>47.92.192.9</host><mode>1</mode><username>tangxuhui</username><password>Tangxuhui0.</password><localpath>/idcdata/surfdata</localpath><remotepath>/tmp/idc/surfdata</remotepath><matchname>SURF_ZH*.XML<SURF_ZH*.CSV</matchname><listfilename>/idcdata/ftplist/ftpgetfiles_surfdata.list</listfilename>\""<<std::endl;
	std::cout<<"本程序是通用的功能模块，用于把远程ftp服务器文件下载到本地"<<std::endl;
        std::cout<<"logfilename是本程序运行的日志文件"<<std::endl;
        std::cout<<"xmlbuffer为文件下载的参数"<<std::endl;
        std::cout<<"host 远程服务器ip地址和端口"<<std::endl;
        std::cout<<"mode 传输模式 1-被动模式 2-主动模式,缺省表示被动模式"<<std::endl;
        std::cout<<"username ftp服务器用户名"<<std::endl;
        std::cout<<"password 密码"<<std::endl;
        std::cout<<"remotepath 远程服务器存放文件的目录"<<std::endl;
        std::cout<<"localpath 本地文件存放的目录"<<std::endl;
        std::cout<<"matchname 带下载文件匹配的规则"<<std::endl;
	std::cout<<"listfilename 下载前列出服务器文件名的文件"<<std::endl;
}

bool _xml2arg(char *strxmlbuffer){
memset(&starg,0,sizeof(struct st_arg));
        GetXMLBuffer(strxmlbuffer,"host",starg.host,30);
        if(strlen(starg.host)==0){
                logfile.Write("host is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"mode",&starg.mode);
        if(starg.mode!=2)
                starg.mode=1;
        GetXMLBuffer(strxmlbuffer,"username",starg.username,30);
        if(strlen(starg.username)==0){
                logfile.Write("username is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"password",starg.password,30);
        if(strlen(starg.password)==0){
                logfile.Write("password is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"remotepath",starg.remotepath,300);
        if(strlen(starg.remotepath)==0){
                logfile.Write("remotepath is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"localpath",starg.localpath,300);
        if(strlen(starg.localpath)==0){
                logfile.Write("localpath is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"matchname",starg.matchname,100);
        if(strlen(starg.matchname)==0){
                logfile.Write("matchname is null.\n");
                return false;
        }
        GetXMLBuffer(strxmlbuffer,"listfilename",starg.listfilename,100);
        if(strlen(starg.matchname)==0){
                logfile.Write("listfilename is null.\n");
                return false;
        }

	return true;
}

bool _ftpgetfiles(){
	// 进入ftp服务器存放文件的目录
	if(ftp.chdir(starg.remotepath)==false){
		logfile.Write("ftp.chdir(%s) failed.\n",starg.remotepath);
		return false;
	}
        // 调用ftp.nlist()方法列出服务器目录中的文件
        if(ftp.nlist(".",starg.listfilename)==false){
		logfile.Write("ftp.nlist(%s) failed.\n",starg.remotepath);
		return false;
	}
        // 把ftp_nlist()方法获取到的list文件加载到容器vfilelist中
        //
        // 遍历容器vfilelist
	/*
        for (int ii=0;ii<vlistfile.size();ii++){
                // 调用ftp.get()方法从服务器下载文件
        }
	*/

	return true;
}
