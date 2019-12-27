#!/bin/bash

standard=( 01.pgm 02.pgm 03.pgm )
tests=( 04.pgm 05.pgm 06.pgm 07.pgm 08.pgm 09.pgm 10.pgm  )

inputFolder="processedFaces"

if ! [[ -d $inputFolder ]] ; then
    echo "ERROR directory $inputFolder not found"
    exit 1
fi

source compressors.sh
source mergers.sh

for comp in ${compressors[@]} ; do
    for testSubjects in $inputFolder/* ; do
        for tst in ${tests[@]} ; do
            echo "Testing:" $testSubjects/$tst
            minNCD=1
            minSub=""
            for goldStdSubjects in $inputFolder/* ; do
                for std in ${standard[@]} ; do
                    append $goldStdSubjects/$std $testSubjects/$tst /tmp/both

                    $comp /tmp/both /tmp/both_compressed
                    $comp $testSubjects/$tst /tmp/testing_compressed
                    $comp $goldStdSubjects/$std /tmp/standard_compressed

                    bothSize=$(stat --printf="%s" /tmp/both_compressed)
                    testingSize=$(stat --printf="%s" /tmp/testing_compressed)
                    standardSize=$(stat --printf="%s" /tmp/standard_compressed)

                    max=$((testingSize > standardSize ? testingSize : standardSize))
                    min=$((testingSize < standardSize ? testingSize : standardSize))

                    NCD=$(echo "scale = 4; ($bothSize - $min) / $max" | bc -l)

                    if (( $(echo "${NCD#-} < ${minNCD#-}" | bc -l) )) ; then  # TODO does this need a abs()?
                        minNCD=$NCD
                        minSub=$goldStdSubjects
                    fi
                done
            done
            echo -e "Result:" $minSub"\n"
        done
    done
done
