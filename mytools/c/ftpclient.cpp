#include<iostream>
#include "_ftp.h"
Cftp ftp;
int main(){
	if(ftp.login("47.92.192.9","tangxuhui","Tangxuhui0.")==false){
		std::cout<<"ftp.login(47.92.192.9:22) failed"<<std::endl;
		return -1;
	}	
	std::cout<<"ftp.login(47.92.192.9:22) success"<<std::endl;
	
	if(ftp.mtime("/datacenter/mytools/ftptest/ftptest.txt")==false){
		std::cout<<"ftp.mtime(datacenter/mytools/ftptest/ftptest.txt) failed"<<std::endl;

	}
	std::cout<<"ftp.mtime(datacenter/mytools/ftptest/ftptest.txt) success mtime="<<ftp.m_mtime<<std::endl;
	if(ftp.size("/datacenter/mytools/ftptest/ftptest.txt")==false){
		std::cout<<"ftp.size(datacenter/mytools/ftptest/ftptest.txt) failed"<<std::endl;
	}
	std::cout<<"ftp.size(datacenter/mytools/ftptest/ftptest.txt) success size="<<ftp.m_size<<std::endl;
	
	ftp.logout();


	return 0;
}

