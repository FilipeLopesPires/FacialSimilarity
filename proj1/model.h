
#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <fstream>
#include <map>
#include <set>

class Model {
    private:
        int ctxLen;                             // context length
        double alpha;                           // smoothing paramenter
        std::ifstream reader;
        std::set<char> abc;                                                    // alphabet of the test data
        std::map<std::string, std::map<char, int>> occurTable;   // table with number of occurrences of each letter after each context present on the test data {context:{letter:num_occur}}

    public:
        Model() {
            ctxLen = 1;
            alpha = 1.0;
            reader.open("text.txt", std::ifstream::in);
        }

        Model(int ctxLen, double alpha) {
            this->ctxLen = ctxLen;
            this->alpha = alpha;
        }

        int getCtxLen() { return ctxLen; }

        double getAlpha() { return alpha; }

        std::map<std::string, std::map<char, int>> getOccurTable() { return occurTable; }

        void fileParser();

        double getModelEntropy() const;
};

#endif