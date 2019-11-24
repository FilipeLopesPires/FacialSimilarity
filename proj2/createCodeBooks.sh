#!/bin/bash
if (( $# != 2 )); then
    echo "Usage: ./createCodeBooks.sh <inputFolder> <outputFolder>"
    exit
fi

for filename in $1/*.wav; do 
    echo "Processing $filename"
    ./build/wavcb "$filename" 800 0.1 0.01 2 "$2/"$(echo $filename | awk -F "[/.]" '{print $2}')
done
