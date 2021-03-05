#!/bin/bash

if [[ $# -ne 1 ]] ; then
    >&2 echo "ERROR: invalid number of arguments"
    >&2 echo "usage: nccdAccuracy.sh <nccdFile>"
    exit 1
fi

correctCount=0
totalCount=0
shopt -s extglob
while read line ; do
    real="$(echo $line | cut -d ':' -f1)"
    if [ "$real" = "Total errors" ] ; then
        continue
    fi
    predicts=$(echo $line | cut -d ':' -f2 | cut -d '-' -f1)
    for p in $predicts;do
        ((totalCount++))
        real=${real##*( )}
        real=${real%%*( )}
        p=${p##*( )}
        p=${p%%*( )}
        if [[ $p == $real ]] ; then
            ((correctCount++))
        fi
    done
done < $1
shopt -u extglob
echo "Accuracy:" $(echo "scale = 3; $correctCount / $totalCount" | bc)
