#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

void func(int num){
	std::cout<<"接收到"<<num<<"信号"<<std::endl;
}
int main(){
	for(int ii=1;ii<=64;ii++)
		signal(ii,func);
		while(1){
			std::cout<<"执行了一次任务"<<std::endl;
			sleep(1);
		}
}
