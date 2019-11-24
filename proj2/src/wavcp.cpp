#include <iostream>
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE =
    65536;  // Buffer for reading/writing frames

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: wavcp <inputFile> <outputFile>" << endl;
        return 1;
    }

    SndfileHandle sndFileIn{argv[argc - 2]};
    checkFileToRead(sndFileIn, argv[argc - 2]);

    cout << "Input file has:" << endl;
    cout << '\t' << sndFileIn.frames() << " frames" << endl;
    cout << '\t' << sndFileIn.samplerate() << " samples per second" << endl;
    cout << '\t' << sndFileIn.channels() << " channels" << endl;

    SndfileHandle sndFileOut{argv[argc - 1], SFM_WRITE, sndFileIn.format(), 1,
                             sndFileIn.samplerate()};
    checkFileOpenSuccess(sndFileOut, argv[argc - 1]);

    size_t nFrames;
    size_t n{};
    short tmpFreq{};

    // read from in, write to out
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFileIn.channels());
    vector<short> mySamples;
    while ((nFrames = sndFileIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        for (auto s : samples) {
            tmpFreq += s;
            if (++n % sndFileIn.channels() == 0) {
                mySamples.push_back(tmpFreq / sndFileIn.channels());
                tmpFreq = 0;
            }
        }
        sndFileOut.writef(mySamples.data(), nFrames);
        mySamples.clear();
    }

    return 0;
}
