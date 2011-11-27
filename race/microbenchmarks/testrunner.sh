#!/bin/bash
# Usage:
# ./testrunner.sh <n> <out>
# 
# n		= number of times the test should be run
# out   = directory for output log files


if [ $# -ne 3 ]; then
    echo "Usage: "
    echo "$0 <threads> <n> <out>"
    echo "threads = number of threads the test can use"
    echo "n   = number of times the test should be run"
    echo "out = directory for output log files"
    exit
fi

threads=$1
n=$2
out=$3

echo "index, mean-time, variance, std-deviation"
elements=`ls -1 test_*.click | cut -d_ -f2,3,4,5 | sed -e 's/\.click//g'`
for k in $elements
do
    click_file="test_$k.click"
    timestamp=`date +"%b_%d_%Y"`
    log_file="test_$k.${timestamp}.out"
    >$out/$log_file
    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $out/$log_file
	/usr/bin/time -v  ../../userlevel/click --threads=$threads $click_file &>> $out/$log_file
    done
    
    mean_time=`cat ${out}/${log_file}| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+" | ./mean -v`
    echo "${k} $mean_time"
done
