#!/bin/bash

# arguments
#  1 -> inputFile
#  2 -> outputFile

_zip() {
    zip -q $2 $1 && mv $2.zip $2
}

_gzip() {
    gzip -fc $1 > $2
}

_lzma() {
    lzma -c $1 > $2
}

# _pax() {
#     pax -wf $2 $1
# }

_bzip2() {
    bzip2 -c $1 > $2
}

_7z() {
    7z a $2 $1 &> /dev/null && mv $2.7z $2
}

# _shar() {
#     shar -q $1 > $2
# }

_zpaq() {
    zpaq a $2 $1 &> /dev/null && mv $2.zpaq $2
}

_ppmd() {
    7z a $2 $1 -m0=PPMd &> /dev/null && mv $2.7z $2
}

# compressors=( _zip _gzip _lzma _pax _bzip2 _7z _shar _zpaq _ppmd )
compressors=( _zip _gzip _lzma _bzip2 _7z _zpaq _ppmd )

