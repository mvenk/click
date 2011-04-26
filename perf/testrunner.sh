#!/bin/bash
# Usage:
# ./testrunner.sh <n> <input_prefix> <conf> <dump_file>
# 
# n		= number of times the test should be run
# input_prefix  = the prefix of the radix1.click file which 
#		  which should be used
# conf		= configuration file which specifies the radix
#		  elements to be used
# dump_file	= The input dump file

if [ $# -ne 4 ]; then
    echo "Usage: "
    echo "$0 <n> <input_prefix> <conf> <dump_file>"
    exit
fi

n=$1
input_prefix=$2
conf=$3
ip_file=$4
echo "index, setup-time(s), setup-memory(kb), radix-time(s),radix-memory(kb),setup-filename,radix-filename"
#./routingtable_replicator.sh $input_prefix $conf $ip_file
elements=`cat $conf | grep -v "#"`
for k in $elements
do
    # Run the setup file.
    j="$input_prefix.setup$k.click"
    >$j.out

    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $j.out
	/usr/bin/time -v  click $j &>> $j.out
    done
    
    setup_mean_time=`cat $j.out| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+"  | ./mean`
    setup_mean_memory=`cat $j.out| grep "Maximum resident" | egrep -o "[0-9]+" | ./mean`

    # Run the radix file.
    j="$input_prefix.radix$k.click"
    >$j.out

    for i in $(seq 1 $n);
    do
	echo "==Run ${i}==">> $j.out
	/usr/bin/time -v  click $j &>> $j.out
    done
    
    radix_mean_time=`cat $j.out| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+"  | ./mean`
    radix_mean_memory=`cat $j.out| grep "Maximum resident" | egrep -o "[0-9]+" | ./mean`
   
    echo "${k}, $setup_mean_time, $setup_mean_memory, $radix_mean_time, $radix_mean_memory, $input_prefix.setup${k}.click, $input_prefix.radix${k}.click"
done