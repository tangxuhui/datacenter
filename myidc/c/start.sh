#!/bin/bash

# 启动数据中心后台服务程序的脚本

# 检测服务程序是否超时
#/datacenter/mytools/bin/procctl 30 /datacenter/mytools/bin/checkproc

# 压缩数据中心后台服务程序的备份日志
/datacenter/mytools/bin/procctl 300 /datacenter/mytools/bin/gzipfiles /log "*.log.20*" 0.03

# 生成用于测试的全国气象站点观测的分钟数据
/datacenter/mytools/bin/procctl 60 /datacenter/myidc/bin/crtsurfdata /datacenter/myidc/ini/stcode.ini /tmp/surfdata /log/crtsurfdata.log xml,json,csv

# 清理原始的全国气象站点观测的分钟数据目录/tmp/surfdata中的所有历史数据文件
/datacenter/mytools/bin/procctl 300 /datacenter/mytools/bin/deletefiles /tmp/surfdata "*" 0.03
