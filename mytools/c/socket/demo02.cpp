#include<iostream>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc,char* argv[]){
	if(argc!=2){
		std::cout<<"Using:./demo02 ip port"<<std::endl;
		std::cout<<"Example:./demo02 5005"<<std::endl;
		return -1;
	}
	// 第一步 创建服务端的socket
	int listenfd;
	if((listenfd = socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		return -1;
	}	
	// 第二步 把服务端用于通讯的地址和端口绑定到socket
	struct sockaddr_in servaddr;   // 服务段地址信息的数据结构
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;   // 协议簇，在socket编程中只能是AF_INET
	servaddr.sin_port = htons(atoi(argv[1]));  
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 任意ip地址
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0){
		perror("bind");
		close(listenfd);
		return -1;
	
	}
	
	// 第三步 把socket设置为监听模式
	if(listen(listenfd,5)!=0){
		perror("listen");
		close(listenfd);
		return -1;

	}
	// 第四步 接受客户端的连接
	int clientfd;
	int socklen = sizeof(struct sockaddr_in); // 
	struct sockaddr_in clientaddr;  // 客户端地址信息
	clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,(socklen_t*)&socklen);
	std::cout<<"客户端"<<inet_ntoa(clientaddr.sin_addr)<<"已连接"<<std::endl;

	int iret;
	char buffer[1024000];
	// 第五步，与客户端通讯，接受客户端发过来的报文后，回复ok
	while(1){
		memset(buffer,0,sizeof(buffer));
		if((iret=recv(clientfd,buffer,sizeof(buffer),0))<=0){ //接收客户端的请求报文
			std::cout<<"iret="<<iret<<std::endl;
			break;
		}
		std::cout<<"接收："<<buffer<<std::endl;
		strcpy(buffer,"ok");
		if((iret=send(clientfd,buffer,strlen(buffer),0))<=0){ // 向客户段发送响应结果
			perror("send");
			break;}
		std::cout<<"发送："<<buffer<<std::endl;
	}


	// 第六步，关闭socket，释放资源
	close(listenfd);
	close(clientfd);

	return 0;
}

