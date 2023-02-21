#include "_public.h"
#include<iostream>
void EXIT(int sig){
	std::cout<<"sig="<<sig<<std::endl;
	exit(0);
}
int main(int argc,char* argv[]){
	if(argc!=3){
		std::cout<<"Using:./book procname timeout"<<std::endl;
		return 0;
	}
	signal(2,EXIT);
	signal(15,EXIT);
	CPActive Active;
	Active.AddPInfo(atoi(argv[2]),argv[1]);
	while(true){
		Active.UptATime();
		sleep(10);
	}


	return 0;
}
