#!/bin/bash

# arguments
#  1 -> inputFile1
#  2 -> inputFile2
#  3 -> outputFile

append() {
    convert $1 $2 -append $3
}

average() {
    convert $1 $2 -average $3
}

mergers=( append average )
