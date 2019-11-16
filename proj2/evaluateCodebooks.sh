#!/bin/bash
if (( $# != 5 )); then
    echo "Usage: ./evaluateCodebooks.sh <blockSize> <overlapFactor> <codebookFolder> <monoFiles> <numTests>"
    exit
fi

filesInFolder=()

for filename in $4/*; do 
    filesInFolder+=($filename)
done

for index in $(seq $5)
do
    file=$((RANDOM % ${#filesInFolder[*]}))
    fileSize=$(sox ${filesInFolder[file]} -n stat 2>&1 | grep Length | awk '{print int($3)}')
    randomStartTest=$((RANDOM % $fileSize))
    randomTestSize=$((1 + RANDOM % $((fileSize-randomStartTest))))
    sox ${filesInFolder[file]} "tmpTest.wav" trim $randomStartTest $randomTestSize
    echo ${filesInFolder[file]} $(./build/wavfind tmpTest.wav $1 $2 $3)
done
rm tmpTest.wav