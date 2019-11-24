
#include <cmath>
#include <iostream>
#include <vector>

#include "headers/io.h"
#include "headers/vctQuant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE =
    65536;  // Buffer for reading/writing frames

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
int main(int argc, const char** argv) {
    if (argc < 3) {
        cout << "Usage: wavcmp <inputFile1> <inputFile2>" << endl
             << "ARGUMENTS:" << endl
             << "   inputFile1 - path to the wav file that will serve as 1st "
                "element of comparisson"
             << endl
             << "   inputFile2 - path to the wav file that will serve as 2nd "
                "element of comparisson"
             << endl;
        exit(1);
    }

    // open origin file
    SndfileHandle sndFileOrig{argv[argc - 2]};
    checkFileToRead(sndFileOrig, argv[argc - 2], 1);

    // open noise file
    SndfileHandle sndFileNoise{argv[argc - 1]};
    checkFileToRead(sndFileNoise, argv[argc - 1], 1);

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
    double Es = calcEs(fileContentOrig);
    double En = calcEn(fileContentOrig, fileContentNoise);
    double SNR = calcSNR(Es, En);

    double enMax = calcEnMax(fileContentOrig, fileContentNoise);

    cout << "SNR: " << SNR << endl;
    cout << "Maximum absolute error: " << enMax << endl;

    return 0;
}

void loadFile(vector<short>& content, SndfileHandle& sndFile) {
    size_t nFrames;
    vector<short> block(FRAMES_BUFFER_SIZE);

    while ((nFrames = sndFile.readf(block.data(), FRAMES_BUFFER_SIZE))) {
        block.resize(nFrames);
        for (auto& sample : block) {
            content.push_back(sample);
        }
    }
}
