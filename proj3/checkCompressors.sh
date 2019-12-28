#!/bin/bash

if [[ $# -ne 1 ]] ; then
    echo "ERROR invalid number of arguments $#, should be 1"
    exit 1
elif ! [[ -f $1 ]] ; then
    echo "ERROR $1 file not found"
    exit 2
fi

source mergers.sh
source compressors.sh

for merger in ${mergers[@]} ; do
    echo ""
    echo "Using $merger"

    $merger $1 $1 /tmp/merged

    for comp in ${compressors[@]} ; do
        echo "Check for $comp"

        $comp $1 /tmp/compressed_single
        if [[ $? -ne 0 ]] ; then
            echo "ERROR on compressor $comp"
            exit 3
        fi

        $comp /tmp/merged /tmp/compressed_merged
        if [[ $? -ne 0 ]] ; then
            echo "ERROR on compressor $comp"
            exit 3
        fi

        echo "single: $(stat --printf="%s" /tmp/compressed_single)"
        echo "merged: $(stat --printf="%s" /tmp/compressed_merged)"
        echo ""
    done

done

