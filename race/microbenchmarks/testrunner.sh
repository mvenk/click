#!/bin/bash
# Usage:
# ./testrunner.sh <n> <out>
# 
# n		= number of times the test should be run
# out   = directory for output log files


if [ $# -ne 3 ]; then
    echo "Usage: "
    echo "$0 <threads> <n> <out>"
    echo "threads = maximum number of threads that the test can use"
    echo "n   = number of times the test should be run"
    echo "out = directory for output log files"
    exit
fi

maxthreads=$1
n=$2
out=$3

echo "index, mean-time, variance, std-deviation"
elements=`ls -1 test_*.click | cut -d_ -f2,3,4 | sed -e 's/\.click//g'`
for k in $elements
do
    reader_threads=`echo $k|cut -d '_' -f 2|cut -d 'r' -f 1`
    writer_threads=`echo $k|cut -d '_' -f 3|cut -d 'w' -f 1`
    threads=$[$writer_threads + $reader_threads]
    if (( threads > maxthreads )); then
	continue
    fi
    click_file="test_$k.click"
    timestamp=`date +"%b_%d_%Y"`
    log_file="test_$k.${timestamp}.out"
    >$out/$log_file
    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $out/$log_file
	/usr/bin/time -p ../../userlevel/click --threads=$threads $click_file 2>> $out/$log_file
    done
    
    mean_time=`cat ${out}/${log_file}| grep "real" | egrep -o "[0-9]+\.[0-9]+" | ./mean -v`
    echo "${k} $mean_time"
done
