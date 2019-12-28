#! /bin/bash

standard=( 01.pgm 02.pgm 03.pgm )
tests=( 04.pgm 05.pgm 06.pgm 07.pgm 08.pgm 09.pgm 10.pgm  )
# convert 01.pgm 06.pgm -fx 'j%2 ? u : v' tmp.pgm

inputfolder="processedFaces"
for subjectt in $inputfolder/*; do
    if [ -d $subjectt ]; then
        for tst in "${tests[@]}"; do
            echo "Testing:" $subjectt"/"$tst
            minimumValue=1000000000
            minimumSub=""
            localMinimum=1000000000
            for subjects in $inputfolder/*; do
                if [ -d $subjects ]; then
                    for std in "${standard[@]}"; do
                        convert $(echo $subjectt"/"$tst) $(echo $subjects"/"$std) -append tmp.pgm
                        zip -q tmp.zip tmp.pgm
                        localMinimum=$(stat --printf="%s" tmp.zip)
                        if [[ $localMinimum -lt $minimumValue ]];then
                            minimumValue=$localMinimum
                            minimumSub=$subjects
                        fi
                    done
                fi
            done
            echo -e "Result:" $minimumSub"\n"
        done
    fi
done