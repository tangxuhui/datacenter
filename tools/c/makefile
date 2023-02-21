# 开发框架头文件路径
PUBINCL =-I/datacenter/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，方便调试
PUBCPP = /datacenter/public/_public.cpp

# 编译参数
CFLAGS = -g


all:ftpgetfiles2 

ftpgetfiles2:ftpgetfiles2.cpp
	g++ -no-pie $(CFLAGS) -o ftpgetfiles2 ftpgetfiles2.cpp $(PUBINCL) $(PUBCPP) /datacenter/public/libftp.a /datacenter/public/_ftp.cpp -lm -lc
	cp ftpgetfiles2 ../bin/.

clean:
	rm -f ftpgetfiles2
