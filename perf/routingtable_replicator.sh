#!/bin/bash
# the input is the prefix of the routing table file
input_prefix="$1"
input_file="$input_prefix.radix.click"
for j in {2..8}
  do
     sed "s/RadixIPLookup/RadixIPLookup$j/g" $input_file > "$input_prefix.radix$j.click" 
     sed "s/5mtest.dump/nothing.dump/g" "$input_prefix.radix$j.click" > "$input_prefix.setup$j.click"
 done

sed "s/5mtest.dump/nothing.dump/g" $input_file > "$input_prefix.setup1.click"
cp $input_file  "$input_prefix.radix1.click"
 