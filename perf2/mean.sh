#!/bin/bash

cat $1| grep "wall clock" | egrep -o "[0-9]+\.[0-9]+"  | ./mean
cat $1| grep "Maximum resident" | egrep -o "[0-9]+" | ./mean