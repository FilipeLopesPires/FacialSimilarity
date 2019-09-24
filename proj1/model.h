
#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <fstream>
#include <map>
#include <set>

class Model {
   private:
    int ctxLen;    // context length
    double alpha;  // smoothing paramenter
    std::fstream *reader;
    std::set<char> abc;  // alphabet of the test data
    std::map<std::string, std::map<char, int>>
        occurTable;  // table with number of occurrences of each letter after
                     // each context present on the test data
                     // {context:{letter:num_occur}}
    void fileParser();

   public:
    Model(int ctxLen, double alpha, std::fstream *input) {
        this->ctxLen = ctxLen;
        this->alpha = alpha;
        this->reader = input;
        this->fileParser();
    }

    int getCtxLen() { return ctxLen; }

    double getAlpha() { return alpha; }

    std::set<char> getABC() { return abc; }

    std::map<std::string, std::map<char, int>> getOccurTable() {
        return occurTable;
    }

    double getModelEntropy() const;

    std::map<std::string, std::map<char, double>> getProbs();
};

#endif