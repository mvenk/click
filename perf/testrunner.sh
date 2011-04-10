#!/bin/bash
conf=$2
if [ $conf == "" ]; then
    conf="default.conf"
fi

files=`cat $conf | grep -v "#"`
echo "N=$1 Conf=$conf"
for j in $files
do
    >$j.out

    for i in $(seq 1 $1);
    do
	echo "==Run ${i}==">> $j.out
	/usr/bin/time -v  click $j &>> $j.out
    done
    
    echo "Mean times for ${j}"
    ./mean.sh $j.out
done
