
#include "headers/vctQuant.h"

#include <cmath>
#include <stdexcept>

using namespace std;

double calcEs(vector<short>& signal) {
    double cumSum = 0;

    for (auto& sample : signal) {
        cumSum += pow(sample, 2);
    }

    return cumSum;
}

double calcEn(vector<short>& original, vector<short>& noise) {
    if (original.size() != noise.size()) {
        throw invalid_argument(
            "Original and noise vectors must"
            " have the same size");
    }

    double cumSum = 0;

    for (size_t i = 0; i < original.size(); i++) {
        short sample1 = original.at(i);
        short sample2 = noise.at(i);

        cumSum += pow(sample1 - sample2, 2);
    }

    return cumSum;
}

double calcEnMax(vector<short>& original, vector<short>& noise) {
    if (original.size() != noise.size()) {
        throw invalid_argument(
            "Original and noise vectors must"
            " have the same size");
    }

    double max = 0;

    for (size_t i = 0; i < original.size(); i++) {
        short sample1 = original.at(i);
        short sample2 = noise.at(i);

        double tmpMax = sample1 - sample2;
        if (abs(tmpMax) > abs(max)) {
            max = tmpMax;
        }
    }

    return max;
}

double calcSNR(double Es, double En) { return En == 0 ? 0 : 10 * log10(Es / En); }

void retrieveBlocks(std::vector<std::vector<short>>& blocks,
                    SndfileHandle& sndFile, int blockSize,
                    float overlapFactor) {
    vector<short> block(blockSize * sndFile.channels());
    int i = 0;
    while (sndFile.readf(block.data(), blockSize) == blockSize) { // Ignore the last block if it
        blocks.push_back(block);                                  //  has a different blockSize
                                                                  // This also stops if no frames are read
        sndFile.seek((blockSize - (int)blockSize * overlapFactor) * i,
                     SEEK_SET);
        i++;
    }
}

double calculateError(vector<vector<short>>& blocks,
                      vector<vector<short>>& codeBook) {
    double totalError{};
    bool first;
    for (auto& block : blocks) {
        first = true;
        double lowerError{};
        for (auto& cb : codeBook) {
            double error = calcEn(block, cb);
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
