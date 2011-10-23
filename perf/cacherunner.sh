#!/bin/bash
# Usage
# ./cacherunner.sh <input_prefix> <conf> <dump_file>
# 
# input_prefix = the prefix of the radix1.click file which
#		 should be used
# conf	       = configuration file which specified the radix
#		 elements to be used
# dump_file      = The input IP dump file

if [ $# -ne 3 ]; then
    echo "Usage: "
    echo "$0 <input_prefix> <conf> <dump_file>"
    exit
fi

input_prefix=$1
conf=$2
ip_file=$3
arch=`python -c "import platform; print platform.architecture()[0][0:2]"`
date=`date +"%b.%d.%Y"`
./routingtable_replicator.sh $input_prefix $conf $ip_file
elements=`cat $conf | grep -v "#"`
out_dir=$conf
for k in $elements
do
    radix="$input_prefix.radix$k.click"
    out="cachegrind.$input_prefix.radix$k.$arch.$date.out"
	mkdir -p out/cache/$arch/$out_dir
    valgrind --tool=cachegrind --cachegrind-out-file=out/cache/$arch/$out_dir/$out ../userlevel/click $radix 
done
