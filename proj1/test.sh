#!/bin/bash
truncate -s 0 test.txt
echo "k,alpha,entropy" >> test.txt
for i in 1 2 3 4 5 6
do
   for f in 0.001 0.01 0.1 0.5 1 2 3
   do
      echo "$i,$f,$(./fcm $i $f 'text.txt' |  tr -dc '.0-9')" >> test.txt
   done
done
