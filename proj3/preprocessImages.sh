#!/bin/sh

inputfolder="orl_faces"
outfolder="processedFaces/"
if [ ! -d $outfolder ];then
    mkdir $outfolder
fi
for subject in $inputfolder/*; do
    if [ -d $subject ]; then
        if [ ! -d $(echo $outfolder$(echo $subject | cut -d "/" -f2)) ]; then
            mkdir $(echo $outfolder$(echo $subject | cut -d "/" -f2))
        fi
        for file in $subject/*; do
            convert $file -resize 46x56 $outfolder$(echo $file | cut -d "/" -f2,3)
        done
    fi
done