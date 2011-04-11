#!/bin/bash
# the input is the prefix of the routing table file
# Usage:
# ./routingtable_replicator.sh <input_prefix> <conf> <ip_file>
# input_pref = The prefix from which the radix1 click file is
#	       derived
# conf	     = Configuration file which contains elements to be executed
# ip_file    = File containing input ip routes

if [ $# -ne 3 ]; then
    echo "Usage: "
    echo "$0 <input_prefix> <conf> <ip_file>"
    exit
fi

input_prefix="$1"
input_file="${input_prefix}.radix1.click"
conf=$2
ip_file=$3

#echo "Input Prefix = $input_prefix, Conf=$conf, IP File=$ip_file"
#echo "Generating Files"
elements=`cat $conf | grep -v "#"`
for j in $elements
  do
    # We don't want to overwrite the radix1.click file
    if [ $input_prefix.radix$j.click == $input_file ]; then
	continue
    fi
	
     sed "s/RadixIPLookup/RadixIPLookup$j/g" $input_file > "$input_prefix.radix$j.click" 
     sed "s/$ip_file/nothing.dump/g" "$input_prefix.radix$j.click" > "$input_prefix.setup$j.click"
 done

sed "s/$ip_file/nothing.dump/g" $input_file > "$input_prefix.setup1.click"

 