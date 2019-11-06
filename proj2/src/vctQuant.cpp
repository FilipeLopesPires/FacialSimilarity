
#include "headers/vctQuant.h"

#include <cmath>
#include <stdexcept>

using namespace std;

long calcEs(vector<short>& signal) {
    long cumSum = 0;

    for (auto& sample : signal) {
        cumSum += pow(sample, 2);
    }

    return cumSum;
}

long calcEn(vector<short>& original, vector<short>& noise) {
    if (original.size() != noise.size()) {
        throw invalid_argument(
            "Original and noise vectors must"
            " have the same size");
    }

    long cumSum = 0;

    for (size_t i = 0; i < original.size(); i++) {
        short sample1 = original.at(i);
        short sample2 = noise.at(i);

        cumSum += pow(sample1 - sample2, 2);
    }

    return cumSum;
}

double calcSNR(long Es, long En) { return En == 0 ? 0 : 10 * log10(Es / En); }

void retrieveBlocks(std::vector<std::vector<short>>& blocks,
                    SndfileHandle& sndFile, int blockSize,
                    float overlapFactor) {
    vector<short> block(blockSize * sndFile.channels());
    int nFrames, i = 0;
    while ((nFrames = sndFile.readf(block.data(), blockSize))) {
        blocks.push_back(block);
        // cout << blocks.at(i).at(0) << endl;
        sndFile.seek((blockSize - (int)blockSize * overlapFactor) * i,
                     SEEK_SET);
        i++;
    }

    // TODO what to do if the last block
    //  does not have blockSize?
}

long calculateError(vector<vector<short>>& blocks,
                    vector<vector<short>>& codeBook) {
    long totalError{};
    bool first;
    for (auto& block : blocks) {
        first = true;
        long lowerError{};
        for (auto& cb : codeBook) {
            long error = calcEn(block, cb);
            if (first) {
                lowerError = error;
                first = false;
            } else {
                if (error < lowerError) {
                    lowerError = error;
                }
            }
        }
        totalError += lowerError;
    }

    return totalError;
}

