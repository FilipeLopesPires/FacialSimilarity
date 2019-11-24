
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"
#include "headers/wavhist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;  // Buffer for reading frames

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: wavhist <inputFile> <channel>" << endl
             << "ARGUMENTS:" << endl
             << "   inputFile - path to the wav file that will serve as base "
                "for the histogram"
             << endl
             << "   channel - the channel that will be represented, 0/1 to "
                "stereo or 0 to mono"
             << endl;
        return 1;
    }

    SndfileHandle sndFile{argv[argc - 2]};
    checkFileToRead(sndFile, argv[argc - 2]);

    int channel{stoi(argv[argc - 1])};
    if (channel >= sndFile.channels() || channel < 0) {
        cerr << "Error: invalid channel requested" << endl;
        return 1;
    }

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    WAVHist hist{sndFile};
    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sndFile.channels());
        hist.update(samples);
    }

    hist.dump(channel);
    hist.fwrite(channel, "histogram.txt");
    //; set yrange [0:45000];replot;
    system("gnuplot -p -e \"plot 'histogram.txt'\"");
    system("rm histogram.txt");

    return 0;
}