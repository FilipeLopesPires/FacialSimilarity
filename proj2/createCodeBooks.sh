#!/bin/bash
if (( $# != 2 )); then
    echo "Usage: ./createCodeBooks.sh <inputFolder> <outputFolder>"
    exit
fi

for filename in $1/*.wav; do 
    echo "Processing $filename"
    ./build/wavcb "$filename" 200 0.1 0.001 "$2/""$(echo $filename | rev | cut -d '/' -f 1 | rev | cut -d '.' -f 1)"
done
