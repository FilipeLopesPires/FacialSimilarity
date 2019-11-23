
#include "headers/io.h"

#include <string>
#include <iostream>

void checkFileToRead(SndfileHandle& sndFile, const char* filename, int channels) {
    checkFileOpenSuccess(sndFile, filename);

    if ((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        std::cerr << "Error: file "
                  << filename
                  << " is not in WAV format" << std::endl;
        exit(3);
    }

    if ((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        std::cerr << "Error: file "
                  << filename
                  << " is not in PCM_16 format" << std::endl;
        exit(3);
    }

    int fileChannels = sndFile.channels();
    if (channels > 0 && channels != fileChannels) {
        std::cerr << "Error: file "
                  << filename
                  << " has " << fileChannels << " channels"
                  << ", must have " << channels
                  << std::endl;
        exit(3);
    }
}

void checkFileOpenSuccess(SndfileHandle& sndFile, const char* filename) {
    if (sndFile.error()) {
        std::cerr << "Error: Some error occur while opening file "
                  << filename
                  << std::endl;
        exit(2);
    }
}
