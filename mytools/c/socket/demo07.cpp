#include "/datacenter/public/_public.h"

int main(int argc,char* argv[]){
	if(argc!=3){
		std::cout<<"Using:./demo07 ip port"<<std::endl;
		std::cout<<"Example:./demo07 127.0.0.1 5005"<<std::endl;
		return -1;
	}
	CTcpClient TcpClient;
	// 向服务端发起连接请求
	if(TcpClient.ConnectToServer(argv[1],atoi(argv[2]))==false){
		std::cout<<"TcpClient.ConnectToServer("<<argv[1]<<argv[2]<<") failed"<<std::endl;
		return -1;
	}
	char buffer[102400];
	// 与服务端通讯，发送一个报文后等待回复，然后再发下一个报文
	for(int i=0;i<10;i++){
		SPRINTF(buffer,sizeof(buffer),"这个是第%d个报文，编号%03d.",i+1,i+1);
		
		if(TcpClient.Write(buffer)==false){  // 向服务端发送请求报文
			break;
		}
		std::cout<<"发送:"<<buffer<<std::endl;
		memset(buffer,0,sizeof(buffer));
		if(TcpClient.Read(buffer)==false) break; // 接收服务端的回应报文
		std::cout<<"接收："<<buffer<<std::endl;
		sleep(1);   // 每隔1s后再次发送报文
	}
}

