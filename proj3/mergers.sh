#!/bin/bash

append() {
    convert $1 $2 -append $3
}

average() {
    convert $1 $2 -average $3
}

mergers=( append average )
