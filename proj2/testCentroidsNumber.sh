#!/bin/bash

alphas=(0.6 0.4 0.2)

for alpha in ${alphas[@]} ; do
    echo "alpha $alpha"
    mkdir "alpha_codebooks/$alpha"
    for f in $(ls wavMonoFiles) ; do
        echo "file $f"
        ./wavcb $alpha "wavMonoFiles/$f" 800 0.3 0.01 2 "alpha_codebooks/$alpha/$f" 1 2
    done
done
