#include "/datacenter/public/_public.h"

int main(int argc,char* argv[]){
	if(argc!=3){
		std::cout<<"Using:./demo01 ip port"<<std::endl;
		std::cout<<"Example:./demo01 127.0.0.1 5005"<<std::endl;
		return -1;
	}
	// 第一步 创建客户端的socket
	int sockfd;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		return -1;
	}	
	// 第二步 向服务器发起来连接请求
	struct hostent* h;
	if((h=gethostbyname(argv[1]))==0){
		std::cout<<"gethostbyname failed"<<std::endl;
		close(sockfd);
		return -1;
	}	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0){
		perror("connect");
		close(sockfd);
		return -1;
	
	}
	int iret;
	char buffer[102400];
	// 第三步 与服务端通讯，连续发送1000个报文
	for(int i=0;i<1000;i++){
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"这是第%d个报文，编号%03d",i+1,i+1);
		if(TcpWrite(sockfd,buffer,strlen(buffer))==false){
			break;
		}
		std::cout<<"发送:"<<buffer<<std::endl;


	}
	// 第四步，关闭socket，释放资源
	close(sockfd);
	return 0;
}

