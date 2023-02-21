#include<iostream>
#include "_ftp.h"
Cftp ftp;
int main(){
	if(ftp.login("47.92.192.9","tangxuhui","Tangxuhui0.")==false){
		std::cout<<"ftp.login(47.92.192.9:22) failed"<<std::endl;
		return -1;
	}	
	std::cout<<"ftp.login(47.92.192.9:22) success"<<std::endl;
	
	if(ftp.nlist("/idcdata/surfdata","/idcdata/111.log")==false){
		std::cout<<"still failed"<<std::endl;
	}	
	if(FtpN)	
	ftp.logout();


	return 0;
}

