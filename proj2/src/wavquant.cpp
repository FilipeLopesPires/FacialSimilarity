
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE =
    65536;  // Buffer for reading/writing frames

// default values for the reduction factor and the number of quantization bits
int REDUCTFACTOR = 1;
int QUANTSIZE = 16;

void parseArguments(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 5 && argc != 7) {
        cerr << "Usage: wavquant [-q <quantSize>] [-r <reductFactor>] "
                "<inputFile> <outputFile>"
             << endl
             << "OPTIONS:" << endl
             << "   quantSize - number of bits that should be used when "
                "quantizing "
                "the values, being 1 the minimun and 16 the maximum"
             << endl
             << "   reductFactor - number of times the sampling rate must be "
                "reduced to, 1 means no reduction, 2 means reduced to half, ..."
             << endl
             << "ARGUMENTS:" << endl
             << "   inputFile - path to the wav file that will be converted"
             << endl
             << "   outputFile - path to the file where the output should be "
                "stored"
             << endl;
        return 1;
    }

    int offset = 0;
    if (argc == 5) {
        offset = 2;
    } else if (argc == 7) {
        offset = 4;
    }

    parseArguments(argc, argv);

    // parse and validate arguments
    const char* inputFileName = argv[1 + offset];
    const char* outputFileName = argv[2 + offset];

    SndfileHandle sndFileIn{inputFileName};
    checkFileToRead(sndFileIn, inputFileName);
    SndfileHandle sndFileOut{outputFileName, SFM_WRITE, sndFileIn.format(), 1,
                             sndFileIn.samplerate() / REDUCTFACTOR};
    checkFileOpenSuccess(sndFileOut, outputFileName);

    // conversion and uniform scalar quantization
    size_t nFrames;
    size_t n{};
    short tmpFreq{};
    vector<short> sampleReduct;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFileIn.channels());
    vector<short> mySamples;
    while ((nFrames = sndFileIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        for (auto s : samples) {
            tmpFreq += s;
            if (++n % sndFileIn.channels() == 0) {
                tmpFreq /=
                    sndFileIn.channels();  // convert multiple channels to one
                sampleReduct.push_back(tmpFreq);
                if (sampleReduct.size() >= REDUCTFACTOR) {
                    tmpFreq =
                        accumulate(begin(sampleReduct), end(sampleReduct), 0) /
                        REDUCTFACTOR;
                    tmpFreq = (tmpFreq &
                               (-1 << sizeof(short) * 8 -
                                          QUANTSIZE));  // remove the QUANTSIZE
                                                        // less significant bits
                    mySamples.push_back(tmpFreq);
                    sampleReduct.clear();
                }
                tmpFreq = 0;
            }
        }
        sndFileOut.writef(mySamples.data(), nFrames / REDUCTFACTOR);
        mySamples.clear();
    }

    return 0;
}

void parseArguments(int argc, char* argv[]) {
    int c;
    while ((c = getopt(argc, (char**)argv, "h:q:r:?")) != -1) {
        switch ((char)c) {
            case 'h':
                cerr
                    << "Usage: wavquant [-q <quantSize>] [-r <reductFactor>] "
                       "<inputFile> <outputFile>"
                    << endl
                    << "OPTIONS:" << endl
                    << "   quantSize - number of bits that should be used when "
                       "quantizing "
                       "the values, being 1 the minimun and 16 the maximum"
                    << endl
                    << "   reductFactor - number of times the sampling rate "
                       "must be "
                       "reduced to, 1 means no reduction, 2 means reduced to "
                       "half, ..."
                    << endl
                    << "ARGUMENTS:" << endl
                    << "   inputFile - path to the wav file that will be "
                       "converted"
                    << endl
                    << "   outputFile - path to the file where the output "
                       "should be "
                       "stored"
                    << endl;
                exit(0);
            case 'q':
                QUANTSIZE = stoi(optarg);
                break;
            case 'r':
                REDUCTFACTOR = stoi(optarg);
                break;
            default:
                cerr << "Error: option " << (char)c << " is invalid." << endl;
                exit(1);
        }
    }

    if (QUANTSIZE <= 0 || QUANTSIZE > 16) {
        cerr << "Error: invalid quantSize: must be between 1 and 16" << endl;
        exit(1);
    }

    if (REDUCTFACTOR <= 0 || REDUCTFACTOR > 16) {
        cerr << "Error: invalid reductFactor: must be between 1 and 16" << endl;
        exit(1);
    }
}
