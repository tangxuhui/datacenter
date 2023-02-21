#!/bin/bash

g++ -g -o ftpclient ftpclient.cpp /datacenter/public/_ftp.cpp /datacenter/public/_public.cpp -I/datacenter/public -L/datacenter/public -lftp -lm -lc
