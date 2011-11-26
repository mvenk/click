#!/bin/bash
# Usage:
# ./testrunner.sh <n> <out>
# 
# n		= number of times the test should be run
# out   = directory for output log files


if [ $# -ne 2 ]; then
    echo "Usage: "
    echo "$0 <n> <out>"
    echo "n   = number of times the test should be run"
    echo "out = directory for output log files"
    exit
fi

n=$1
out=$2

echo "index, mean-time, variance, std-deviation"
elements=`ls -1 167k_test_*.click | cut -d_ -f3,4,5 | sed -e 's/\.click//g'`
for k in $elements
do
    click_file="167k_test_$k.click"
    timestamp=`date +"%b_%d_%Y"`
    log_file="167k_test_$k.${timestamp}.out"
    >$out/$log_file
    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $out/$log_file
	/usr/bin/time -v  ../../userlevel/click --threads=4 $click_file &>> $out/$log_file
    done
    
    mean_time=`cat ${out}/${log_file}| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+" | ../mean -v`
    echo "${k} $mean_time"
done
