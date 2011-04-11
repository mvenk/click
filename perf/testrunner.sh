#!/bin/bash
# Usage:
# ./testrunner.sh <n> <input_prefix> <conf> <ip_file>
# 
# n		= number of times the test should be run
# input_prefix  = the prefix of the radix1.click file which 
#		  which should be used
# conf		= configuration file which specifies the radix
#		  elements to be used
# ip_file	= The input IP dump file

if [ $# -ne 4 ]; then
    echo "Usage: "
    echo "$0 <n> <input_prefix> <conf> <ip_file>"
    exit
fi

n=$1
input_prefix=$2
conf=$3
ip_file=$4
echo "N=$n, Input Prefix=$input_prefix, Conf=$conf, IP File=$ip_file"
echo "Generating click script files"
./routingtable_replicator.sh $input_prefix $conf $ip_file
setup_files=`cat $conf | grep -v "#" | xargs printf "$input_prefix.setup%d.click\n"`
radix_files=`cat $conf | grep -v "#" | xargs printf "$input_prefix.radix%d.click\n"`
for j in $setup_files $radix_files
do
    >$j.out

    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $j.out
	/usr/bin/time -v  click $j &>> $j.out
    done
    
    echo "Mean times for ${j}"
    ./mean.sh $j.out
done
