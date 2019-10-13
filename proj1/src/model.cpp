
#include "model.h"

#include <iostream>
#include <list>

using namespace std;
using namespace model;

void Model::parseFile(list<fstream *> &input) {
    char letter;
    string context;

    //for each file
    for (auto reader : input) {
        while (reader->get(letter)) {
            // Note: abc is a Set
            abc.insert(letter);

            // if the current sliding window length is the size of
            //  defined context length
            if (context.length() >= ctxLen) {
                // increment the count of the current letter after the current context
                statsTable[context].nextCharStats[letter].count++;

                // increment the count of the current context
                statsTable[context].stats.count++;

                // increment total count of all contexts
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

    // for each context
    for (auto &it : statsTable) {
        contextStats = &it.second;

        contextCount = contextStats->stats.count;
        // calculate the probability for the context
        contextStats->stats.probability =
            (double)contextCount / totalContextsCount;

        // a temporary copy of the alphabet
        set<char> abcCopy(abc);

        // for each letters' occurrences statistics
        for (auto &it2 : contextStats->nextCharStats) {
            letter = it2.first;
            stats = &it2.second;

            charCount = stats->count;
            // calculate their occurrence probability after the current context
            conditionalProb =
                (charCount + alpha) / (contextCount + alpha * abc.size());
            stats->probability = conditionalProb;

            // update the context entropy
            Hc += conditionalProb * log2(conditionalProb);

            abcCopy.erase(letter);
        }

        // for each letter of the alphabet that didn't appear after
        //  the current context
        // we do this mainly for the text generation process so
        //  we know the probability, affected by the smoothing parameter,
        //  of the letters that don't appear after a given context
        for (char l : abcCopy) {
            // insert them with occurrence count as 0
            conditionalProb = alpha / (contextCount + (alpha * abc.size()));
            contextStats->nextCharStats[l] = {0, conditionalProb};
            if (conditionalProb > 0) {
                Hc += conditionalProb * log2(conditionalProb);
            }
        }

        Hc = -Hc;
        entropy += contextStats->stats.probability * Hc;

        Hc = 0.0;
    }
}
