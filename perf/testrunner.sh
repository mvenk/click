#!/bin/bash
for j in 1m_gt20.setup[1,6].click 1m_gt20.radix[1,6].click 
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
