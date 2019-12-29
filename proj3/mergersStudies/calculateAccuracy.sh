#!/bin/bash

if [[ $# -ne 1 ]] ; then
    >&2 echo "ERROR: invalid number of arguments"
    >&2 echo "usage: calculateAccuracy.sh <studyFile>"
    exit 1
fi

compressors=()
accuracies=()

rightCount=0
totalCount=0

readHeader=0
while read line ; do
    if [[ readHeader -eq 0 ]] ; then
        readHeader=1
        continue
    fi

    if [[ ${line:0:1} = "_" ]] ; then
        if [[ ${#compressors[@]} -ne 0 ]] ; then
            acc=$(echo "scale = 3; $rightCount / $totalCount" | bc)
            accuracies+=($acc)
            rightCount=0
            totalCount=0
        fi

        compressors+=($line)
    else
        real=$(echo $line | cut -d "," -f2)
        predict=$(echo $line | cut -d "," -f3)

        if [[ $real = $predict ]] ; then
            ((rightCount++))
        fi

        ((totalCount++))
    fi
done < $1

acc=$(echo "scale = 3; $rightCount / $totalCount" | bc)
accuracies+=($acc)

for i in $(seq 0 $((${#compressors[@]} - 1))) ; do
    echo "${compressors[$i]}: ${accuracies[$i]}"
done
