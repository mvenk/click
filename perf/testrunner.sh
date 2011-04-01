#!/bin/bash
for j in setup[127].click radix[127].click 
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
