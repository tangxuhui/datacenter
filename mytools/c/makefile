# 开发框架头文件路径
PUBINCL =-I/datacenter/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，方便调试
PUBCPP = /datacenter/public/_public.cpp

# 编译参数
CFLAGS = -g


all: procctl checkproc gzipfiles deletefiles ftpgetfiles2 test

procctl:procctl.cpp
	g++ -o procctl procctl.cpp
	cp procctl ../bin/.

checkproc:checkproc.cpp
	g++ $(CFLAGS) -o checkproc checkproc.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp checkproc ../bin/.
gzipfiles:gzipfiles.cpp
	g++ $(CFLAGS) -o gzipfiles gzipfiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp gzipfiles ../bin/.
deletefiles:deletefiles.cpp
	g++ $(CFLAGS) -o deletefiles deletefiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc 
	cp deletefiles ../bin/.
#ftpclient:ftpclient.cpp
#	g++ $(CFLAGS) -o ftpclient ftpclient.cpp $(PUBINCL) $(PUBCPP) -lm -lc
#	cp ftpclient ../bin/.

ftpgetfiles2:ftpgetfiles2.cpp
	g++ -no-pie $(CFLAGS) -o ftpgetfiles2 ftpgetfiles2.cpp $(PUBINCL) $(PUBCPP) /datacenter/public/libftp.a /datacenter/public/_ftp.cpp -lm -lc
	cp ftpgetfiles2 ../bin/.
test:test.cpp
	g++ -no-pie $(CFLAGS) -o test test.cpp $(PUBINCL) $(PUBCPP) /datacenter/public/libftp.a /datacenter/public/_ftp.cpp -lm -lc
	cp ftpgetfiles2 ../bin/.
clean:
	rm -f procctl checkproc gzipfiles deletefiles ftpgetfiles2
