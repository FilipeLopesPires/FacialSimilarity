#!/bin/bash
if (( $# != 3 )); then
    echo "Usage: ./filesQuant.sh <inputFolder> <outputFolder> <quantSize>"
    exit
fi

for filename in $1/*.wav; do 
    echo "Processing $filename"
    ./build/wavquant $3 "$filename" "$2/""$(echo $filename | rev | cut -d '/' -f 1 | rev)"
done