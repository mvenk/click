#!/bin/bash
# Usage
# ./cacherunner.sh <input_prefix> <conf> <ip_file>
# 
# input_prefix = the prefix of the radix1.click file which
#		 should be used
# conf	       = configuration file which specified the radix
#		 elements to be used
# ip_file      = The input IP dump file

if [ $# -ne 3 ]; then
    echo "Usage: "
    echo "$0 <input_prefix> <conf> <ip_file>"
    exit
fi

input_prefix=$1
conf=$2
ip_file=$3

./routingtable_replicator.sh $input_prefix $conf $ip_file
elements=`cat $conf | grep -v "#"`
for k in $elements
do
    radix="$input_prefix.radix$k.click"
    out="cachegrind.$input_prefix.radix$k.out"
    valgrind --tool=cachegrind --cachegrind-out-file=$out ../userlevel/click $radix 
done
