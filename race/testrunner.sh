#!/bin/bash
# Usage:
# ./testrunner.sh <n>
# 
# n		= number of times the test should be run


if [ $# -ne 1 ]; then
    echo "Usage: "
    echo "$0 <n> <input_prefix> <conf> <dump_file>"
    exit
fi

n=$1
echo "index, mean-time"
elements=`ls -l test_*.click|wc -l`
for k in $(seq 1 $elements)
do
    j="test_$k.click"
    >$j.out
    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $j.out
	/usr/bin/time -v  ../userlevel/click --threads=4 $j &>> $j.out
    done
    
    mean_time=`cat $j.out| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+"  | ./mean`  
    echo "${k}, $mean_time"
done