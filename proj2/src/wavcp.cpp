#include <iostream>
#include <sndfile.hh>
#include <vector>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE =
    65536;  // Buffer for reading/writing frames

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: wavcp <inputFile> <outputFile>" << endl;
        return 1;
    }

    SndfileHandle sndFileIn{argv[argc - 2]};
    if (sndFileIn.error()) {
        cerr << "Error: invalid inputFile" << endl;
        return 1;
    }

    if ((sndFileIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format" << endl;
        return 1;
    }

    if ((sndFileIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format" << endl;
        return 1;
    }

    cout << "Input file has:" << endl;
    cout << '\t' << sndFileIn.frames() << " frames" << endl;
    cout << '\t' << sndFileIn.samplerate() << " samples per second" << endl;
    cout << '\t' << sndFileIn.channels() << " channels" << endl;

    SndfileHandle sndFileOut{argv[argc - 1], SFM_WRITE, sndFileIn.format(), 1,
                             sndFileIn.samplerate()};
    if (sndFileOut.error()) {
        cerr << "Error: invalid outputFile" << endl;
        return 1;
    }

    // size_t nFrames;
    // vector<short> samples(FRAMES_BUFFER_SIZE * sndFileIn.channels());
    // while ((nFrames = sndFileIn.readf(samples.data(), FRAMES_BUFFER_SIZE)))
    //     sndFileOut.writef(samples.data(), nFrames);

    size_t nFrames;
    size_t n{};
    short tmpFreq{};

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
