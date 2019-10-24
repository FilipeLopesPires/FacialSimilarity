
#include <cmath>
#include <vector>
#include <iostream>

#include "headers/snr.h"
#include "headers/io.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

/*!
 * Loads the content of a file to
 *  the received vector
 *
 * @param content where to store the file's content
 * @param sndFile the file to read from
 */
void loadFile(vector<short>& content, SndfileHandle& sndFile);

/**
 * Calculates the SNR of a certain audio file
 *  in relation to the original file.
 *
 * Exit codes:
 * <ul>
 *  <li>0: executed without errors</li>
 *  <li>1: arguments errors</li>
 *  <li>2: io errors</li>
 *  <li>3: wrong file format</li>
 *  <li>4: file's content with different sizes</li>
 * </ul>
 */
int main(int argc, const char **argv) {
    if (argc < 3) {
        cout << "Usage: wavcmp <origin file> <noise file>" << endl;
        exit(1);
    }

    // open origin file
    string origFilename = argv[argc - 2];
    SndfileHandle sndFileOrig { origFilename };
    checkFile(sndFileOrig, origFilename, true);

    // open noise file
    string noiseFilename = argv[argc - 1];
    SndfileHandle sndFileNoise { noiseFilename };
    checkFile(sndFileNoise, noiseFilename, true);

    // load origin file
    vector<short> fileContentOrig;
    loadFile(fileContentOrig, sndFileOrig);

    // load noise file
    vector<short> fileContentNoise;
    loadFile(fileContentNoise, sndFileNoise);

    // check if files's content size match
    if (fileContentOrig.size() != fileContentNoise.size()) {
        std::cerr << "Error: received files "
                  << "don't have the same number of samples" << std::endl;
        exit(4);
    }

    // calculate snr
    long Es = calcEs(fileContentOrig);
    long En = calcEn(fileContentOrig, fileContentNoise);

    double SNR = calcSNR(Es, En);

    cout << "SNR: " << SNR << endl;

    return 0;
}

void loadFile(vector<short>& content, SndfileHandle& sndFile) {
    size_t nFrames;
    vector<short> block(FRAMES_BUFFER_SIZE);

    while((nFrames = sndFile.readf(block.data(), FRAMES_BUFFER_SIZE))) {
        block.resize(nFrames);
        for (auto& sample : block) {
            content.push_back(sample);
        }
    }
}

