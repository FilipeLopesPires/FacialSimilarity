#! /bin/bash

standard=( 01.pgm 02.pgm 03.pgm )
tests=( 04.pgm 05.pgm 06.pgm 07.pgm 08.pgm 09.pgm 10.pgm  )

compressor=( zip gzip lzma xz bzip2 pax 7z shar zpaq )

compress() {
    if [ "$2" = "zip" ]; then 
        $2 -q tmp $1
        mv tmp.zip tmp
    fi
    if [ "$2" = "lzma" ] || [ "$2" = "gzip" ] || [ "$2" = "xz" ] || [ "$2" = "bzip2" ]; then
        $2 -c $1 > tmp
    fi
    if [ "$2" = "pax" ]; then
        $2 -wf tmp $1
    fi
    if [ $2 = "shar" ]; then
        $2 -q $1 > tmp
    fi
    if [ $2 = "7z" ]; then
        $2 a tmp $1 1>/dev/null
        mv tmp.7z tmp
    fi
    if [ $2 = "zpaq" ]; then
        $2 a tmp $1 &>/dev/null
        mv tmp.zpaq tmp
    fi
}

inputfolder="processedFaces"
for comp in $compressor; do
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
                            compress tmp.pgm $comp
                            localMinimum=$(stat --printf="%s" tmp)
                            if [[ $localMinimum -lt $minimumValue ]]; then
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
done

rm tmp
rm tmp.pgm
