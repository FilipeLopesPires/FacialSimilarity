#!/bin/bash

if [[ $# -ne 1 ]] ; then
    >&2 echo "ERROR: invalid number of arguments"
    >&2 echo "usage: calculateAccuracy.sh <studyFile>"
    exit 1
fi

correctCount=0
totalCount=0

readHeader=0
firstCompressor=1
while read line ; do
    if [[ readHeader -eq 0 ]] ; then
        readHeader=1
        continue
    fi

    if [[ ${line:0:1} = "_" ]] ; then
        if [[ $firstCompressor -eq 0 ]] ; then
            echo "$(echo "scale = 3; $correctCount / $totalCount" | bc)"

            correctCount=0
            totalCount=0
        else
            firstCompressor=0
        fi

        echo -n "$(echo $line | cut -d '_' -f2): "
    else
        real=$(echo $line | cut -d "," -f2)
        predict=$(echo $line | cut -d "," -f3)

        if [[ $real = $predict ]] ; then
            ((correctCount++))
        fi

        ((totalCount++))
    fi
done < $1

echo $(echo "scale = 3; $correctCount / $totalCount" | bc)
