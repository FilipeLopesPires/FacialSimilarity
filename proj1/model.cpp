
#include "model.h"

#include <iostream>
#include <list>

using namespace std;

void Model::parseFile(list<fstream*> &input) {
    char letter;
    string context;

    for (auto reader : input) {
        while (reader->get(letter)) {
            abc.insert(letter);

            if (context.length() >= ctxLen) {
                statsTable[context].nextCharStats[letter].count++;

                statsTable[context].stats.count++;

                totalContextsCount++;

                context = context.substr(1);
            }
            context += letter;
        }
    }

    calcProbabilitiesAndEntropy();
}

void Model::calcProbabilitiesAndEntropy() {
    int contextCount;
    int charCount;

    ContextStatistics *contextStats;

    char letter;
    Statistics *stats;
    double conditionalProb;

    double Hc = 0.0;

    entropy = 0;

    for (auto &it : statsTable) {
        contextStats = &it.second;

        contextCount = contextStats->stats.count;
        contextStats->stats.probability =
            (double)contextCount / totalContextsCount;

        set<char> abcCopy(abc);

        for (auto &it2 : contextStats->nextCharStats) {
            letter = it2.first;
            stats = &it2.second;

            charCount = stats->count;
            conditionalProb =
                (charCount + alpha) / (contextCount + alpha * abc.size());
            stats->probability = conditionalProb;

            Hc += conditionalProb * log2(conditionalProb);

            abcCopy.erase(letter);
        }

        for (char l : abcCopy) {
            conditionalProb = alpha / (contextCount + (alpha * abc.size()));
            contextStats->nextCharStats[l] = {0, conditionalProb};
            if (conditionalProb > 0) {
                Hc += conditionalProb * log2(conditionalProb);
            }
        }

        entropy += contextStats->stats.probability * -Hc;

        Hc = 0.0;
    }
}
