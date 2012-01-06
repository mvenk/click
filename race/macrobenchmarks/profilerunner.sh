#!/bin/bash
# Usage:
# ./testrunner.sh <n> <out>
# 
# n		= number of times the test should be run
# out   = directory for output log files


if [ $# -ne 6 ]; then
    echo "Usage: "
    echo "$0 <threads> <n> <out>"
    echo "threads = maximum number of threads that the test can use"
    echo "n   = number of times the test should be run"
    echo "out = directory for output log files"
    echo "prefix = the prefix of the test to run"
    echo "reader count= 0 to maxthreds, v if it varies"
    echo "writer count = 0 to maxthreads, v if it varies"
    exit
fi

maxthreads=$1
n=$2
out=$3
prefix=$4
if [ $5 == 'v' ]; then
    minreaders=1
    maxreaders=$maxthreads
else
    minreaders=$5
    maxreaders=$(($5 +1))
fi

if [ $6 == 'v' ]; then
    minwriters=1
    maxwriters=$maxthreads
else
    minwriters=$6
    maxwriters=$(($6 +1))
fi
echo "index, mean-time, variance, std-deviation"
for (( j = $minreaders; j < $maxreaders; j++ ))
do
    reader_threads=$j
    for (( m = $minwriters; m < $maxwriters; m++ ))
    do
	writer_threads=$m
	threads=$[$writer_threads + $reader_threads]
	k="${prefix}_${reader_threads}r_${writer_threads}w"
	if (( threads > maxthreads )); then
	    continue
	fi
	click_file="167k_test_$k.click"
	timestamp=`date +"%b_%d_%Y"`
	log_file="167k_profile_$k.out"
	>$out/$log_file
	for i in $(seq 1 $n);
	do
	    sudo ./hotuser -c "../../userlevel/click --threads=$threads $click_file" >> $out/$log_file
	done
    done
done