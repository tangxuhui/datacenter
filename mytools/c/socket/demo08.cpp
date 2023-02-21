#include "/datacenter/public/_public.h"

int main(int argc,char* argv[]){
	if(argc!=2){
		std::cout<<"Using:./demo08 port"<<std::endl;
		std::cout<<"Example:./demo08 5005"<<std::endl;
		return -1;
	}
	
	CTcpServer TcpServer;
	// 服务端初始化
	if(TcpServer.InitServer(atoi(argv[1]))==false){
		std::cout<<"TcpServer.InitServer"<<argv[1]<<" failed"<<std::endl;
		return -1;
	}
	
	// 等待客户端的连接请求
	if(TcpServer.Accept()==false){
		std::cout<<"TcpServer.Accept() failed"<<std::endl;
		return -1;
	}
	std::cout<<"客户端"<<TcpServer.GetIP()<<"已连接"<<std::endl;
	char buffer[102400];
	// 与客户端通讯，接收客户端发过来的报文后，回复ok
	while(1){
		memset(buffer,0,sizeof(buffer));
		if(TcpServer.Read(buffer)==false) break; // 接收客户端的请求报文
		std::cout<<"接收："<<buffer<<std::endl;

		strcpy(buffer,"ok");
		if(TcpServer.Write(buffer)==false) break;  // 向客户端发送发送响应结果
		std::cout<<"发送"<<buffer<<std::endl;
	}
	
	
}

