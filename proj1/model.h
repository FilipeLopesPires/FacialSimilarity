
#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <fstream>
#include <list>
#include <map>
#include <set>

typedef struct {
    int count;
    double probability;
} Statistics;

typedef struct {
    std::map<char, Statistics> nextCharStats;
    Statistics stats;
} ContextStatistics;

class Model {
   private:
    /**
     * context length
     */
    int ctxLen;

    /**
     * smoothing paramenter
     */
    double alpha;

    /**
     * alphabet of the test data
     */
    std::set<char> abc;

    int totalContextsCount;

    /**
     * table with number of occurrences of each letter after
     * each context present on the test data
     * {context:{letter:num_occur}}
     */
    std::map<std::string, ContextStatistics> statsTable;

    double entropy;

    void calcProbabilitiesAndEntropy(std::set<char>& lettersNotChanged);

   public:
    Model(int ctxLen, double alpha) {
        this->ctxLen = ctxLen;
        this->alpha = alpha;
    }

    auto& getABC() const { return abc; }

    auto& getStatsTable() const { return statsTable; }

    auto getModelEntropy() { return entropy; };

    void parseFile(std::list<std::fstream*>& input);
};

#endif