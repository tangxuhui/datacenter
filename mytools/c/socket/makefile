# 开发框架头文件路径
PUBINCL = -I/datacenter/public

# 开发框架cpp文件名
PUBCPP = /datacenter/public/_public.cpp

# 编译参数
CFLOGS = -g

all:demo01 demo02 demo03 demo04 demo07 demo08

demo01:demo01.cpp
	g++ $(CFLOGS) -o demo01 demo01.cpp $(PUBINCL) $(PUBCPP) -lm -lc
demo02:demo02.cpp
	g++ $(CFLOGS) -o demo02 demo02.cpp $(PUBINCL) $(PUBCPP) -lm -lc
demo03:demo03.cpp
	g++ $(CFLOGS) -o demo03 demo03.cpp $(PUBINCL) $(PUBCPP) -lm -lc
demo04:demo04.cpp
	g++ $(CFLOGS) -o demo04 demo04.cpp $(PUBINCL) $(PUBCPP) -lm -lc
demo07:demo07.cpp
	g++ $(CFLOGS) -o demo07 demo07.cpp $(PUBINCL) $(PUBCPP) -lm -lc
demo08:demo08.cpp
	g++ $(CFLOGS) -o demo08 demo08.cpp $(PUBINCL) $(PUBCPP) -lm -lc
clean:
	rm demo01 demo02 demo03 demo04 demo07 demo08
