
#include "headers/snr.h"

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
        throw invalid_argument("Original and noise vectors must"
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

double calcSNR(long Es, long En) {
    return En == 0 ? 0 : 10 * log10(Es / En);
}
