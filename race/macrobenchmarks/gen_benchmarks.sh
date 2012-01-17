#!/bin/bash
# Usage:
# ./testrunner.sh <n> <out>
# 
# n             = number of times the test should be run
# out   = directory for output log files


if [ $# -ne 5 ]; then
    echo "Usage: "
    echo "$0 <threads> <prefix> <reader count> <writer count> <type>"
    echo "threads = maximum number of threads that the test can use"
    echo "prefix = the prefix of the test to run"
    echo "reader count= 0 to maxthreds, v if it varies"
    echo "writer count = 0 to maxthreads, v if it varies"
    echo "type = type of benchmark m/M"
    exit
fi

maxthreads=$1
prefix=$2
type=$5
if [ $3 == 'v' ]; then
    minreaders=1
    maxreaders=$maxthreads
else
    minreaders=$3
    maxreaders=$(($3))
fi

if [ $4 == 'v' ]; then
    minwriters=1
    maxwriters=$maxthreads
else
    minwriters=$4
    maxwriters=$(($4))
fi
echo "index, mean-time, variance, std-deviation"
for (( j = $minreaders; j <= $maxreaders; j++ ))
do
    reader_threads=$j
    for (( m = $minwriters; m <= $maxwriters; m++ ))
    do
        writer_threads=$m
        threads=$[$writer_threads + $reader_threads]
        k="${prefix}_${reader_threads}r_${writer_threads}w"
        if (( threads > maxthreads )); then
            continue
        fi
	python test_gen.py -r $reader_threads -w $writer_threads --type=$type --sync=$prefix
    done
done