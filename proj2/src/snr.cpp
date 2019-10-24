
#include "headers/snr.h"

#include <cmath>
#include <stdexcept>

using namespace std;

long calcEs(vector<short>& vector) {
    long cumSum = 0;

    for (auto& sample : vector) {
        cumSum += pow(sample, 2);
    }

    return cumSum;
}

long calcEn(vector<short>& vector1, vector<short>& vector2) {
    if (vector1.size() == vector2.size()) {
        throw invalid_argument("Vector 1 and 2 must"
                               " have the same size");
    }

    long cumSum = 0;

    for (size_t i = 0; i < vector1.size(); i++) {
        short sample1 = vector1.at(i);
        short sample2 = vector2.at(i);

        cumSum += pow(sample1 - sample2, 2);
    }

    return cumSum;
}

double calcSNR(long Es, long En) {
    return En == 0 ? 0 : 10 * log10(Es / En);
}
