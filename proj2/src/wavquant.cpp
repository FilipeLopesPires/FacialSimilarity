#include <bitset>
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

SndfileHandle parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn);

int main(int argc, char *argv[]) {
    if (argc < 3 || (argc != 5 && argc != 7)) {
        cerr << "Usage: wavquant inputFile outputFile [-q quantSize] [-r reductFactor]"
             << endl;
        return 1;
    }

    // parse and validate arguments
    SndfileHandle sndFileIn{argv[1]};
    SndfileHandle sndFileOut = parseArguments(argc, argv, sndFileIn);

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
                tmpFreq /= sndFileIn.channels();
                tmpFreq = (tmpFreq & (-1 << sizeof(short) * 8 - QUANTSIZE));
                sampleReduct.push_back(tmpFreq);
                if (sampleReduct.size() >= REDUCTFACTOR) {
                    mySamples.push_back(
                        accumulate(begin(sampleReduct), end(sampleReduct), 0) /
                        REDUCTFACTOR);
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

SndfileHandle parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn) {

    // this could be optimized...
    if (argc > 3) {
        string option = argv[3];
        if (option != "-q" && option != "-r") {
            cerr << "Error: invalid option definition: must be either '-q' or '-r'" << endl;
            exit(1);
        }

        if (option == "-q") {
            int quantSize = stoi(argv[4]);
            if (quantSize <= 0 || quantSize > 16) {
                cerr << "Error: invalid quantSize: must be between 1 and 16" << endl;
                exit(1);
            }
            QUANTSIZE = quantSize;

            if (argc == 7) {
                option = argv[5];
                if (option != "-r") {
                    cerr << "Error: invalid option definition: must be either '-q' or '-r' (and you cannot repeat them)" << endl;
                    exit(1);
                }
                int reductFactor = stoi(argv[6]);
                if (reductFactor <= 0 || reductFactor > 16) {
                    cerr << "Error: invalid reductFactor: must be between 1 and 16" << endl;
                    exit(1);
                }
                REDUCTFACTOR = reductFactor;
            }
        } else {
            int reductFactor = stoi(argv[4]);
            if (reductFactor <= 0 || reductFactor > 16) {
                cerr << "Error: invalid reductFactor: must be between 1 and 16" << endl;
                exit(1);
            }
            REDUCTFACTOR = reductFactor;

            if (argc == 7) {
                option = argv[5];
                if (option != "-q") {
                    cerr << "Error: invalid option definition: must be either '-q' or '-r' (and you cannot repeat them)" << endl;
                    exit(1);
                }
                int quantSize = stoi(argv[6]);
                if (quantSize <= 0 || quantSize > 16) {
                    cerr << "Error: invalid quantSize: must be between 1 and 16" << endl;
                    exit(1);
                }
                QUANTSIZE = quantSize;
            }
        }
    }

    checkFileToRead(sndFileIn, argv[1]);

    SndfileHandle sndFileOut{argv[2], SFM_WRITE, sndFileIn.format(), 1, sndFileIn.samplerate() / REDUCTFACTOR};
    checkFileOpenSuccess(sndFileOut, argv[2]);

    return sndFileOut;
}
