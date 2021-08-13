#!/bin/bash

export CPULOOP=1

checknu=`ps -ef| grep perfstat | grep -v grep |wc -l`
day=`date +%Y%m%d`
week=`date +%w`
if [ "$week" == "6" ] || [ "$week" == "0" ]
then
    exit
fi

time=`date '+%H%M'`
if [ "$time" -gt "2330" ] || [ "$time" -lt "700" ]
then
    exit
fi

if [ "$checknu" -lt "4" ]
then
    while true
    do
        time=`date '+%H%M'`
        if [ "$time" -gt "2330" ] || [ "$time" -lt "700" ]
        then
            exit
        fi
        #hexincs pid
        cs_pid=`ps aux | grep hexincs | grep -v grep | awk '{print $2}'`
        #echo "cs pid:"$cs_pid
        #top cpu
        sycpu=`top -bn 1 | grep -i "Cpu(s)" | awk -F"us," '{print strtonum($2)}'`
        #echo "sy cpu top:"$sycpu
        
        if [ -n "$cs_pid" ] && [ $(bc -l <<< "$sycpu>40") -eq 1 ]
        then
            second=`date '+%H%M%S'`

            #pstack
            pstack $cs_pid > /hxdata/pstack_${day}_${second}.ps
            sleep 2
            pstack $cs_pid > /hxdata/pstack_${day}_${second}.ps

            #perf
            perf record -o /hxdata/perf_${day}_${second}.data -p $cs_pid  -g -q sleep 6

            #pstack
            pstack $cs_pid > /hxdata/pstack_${day}_${second}.ps
            sleep 30
        fi

        sleep 5
     done
fi

