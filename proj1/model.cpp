
#include "model.h"

#include <algorithm>

using namespace std;

void Model::parseFile(fstream &reader) {
    char letter;
    string context;

    // abc is the current alphabet
    set<char> oldAbc(abc); // copy of the current abc
    set<char> newAbc;      // abc of the file being parsed

    while (reader.get(letter)) {
        abc.insert(letter);
        newAbc.insert(letter);

        if (context.length() >= ctxLen) {
            statsTable[context].nextCharStats[letter].count++;

            statsTable[context].stats.count++;

            totalContextsCount++;

            context = context.substr(1);
        }

        context += letter;
    }

    set<char> lettersNotChanged;
    set_difference(
            oldAbc.begin(), oldAbc.end(),
            newAbc.begin(), newAbc.end(),
            inserter(lettersNotChanged, lettersNotChanged.begin())
    );
    //This difference give us the letters that were present on the
    // previous/old alphabet and were not seen on the new file
    //This is calculated to know which letters, in all contexts,
    // we need to update the conditional probabilities, since the number
    // of times a context appear can change

    calcProbabilitiesAndEntropy(lettersNotChanged);
}

void Model::calcProbabilitiesAndEntropy(set<char> &lettersNotChanged) {
    int contextCount;
    int charCount;

    ContextStatistics* contextStats;

    char letter;
    Statistics* stats;
    double conditionalProb;

    double Hc = 0.0;

    entropy = 0;

    for (auto &it : statsTable) {
        contextStats = &it.second;

        contextCount = contextStats->stats.count;
        contextStats->stats.probability = (double) contextCount / totalContextsCount;

        set<char> abcCopy(abc);

        for (auto &it2 : contextStats->nextCharStats) {
            letter = it2.first;
            stats = &it2.second;

            charCount = stats->count;
            conditionalProb = (charCount + alpha) /
                              (contextCount + alpha * abc.size());
            stats->probability = conditionalProb;

            Hc += conditionalProb * log2(conditionalProb);

            abcCopy.erase(letter);
        }
        Hc = -Hc; 
        entropy += contextStats->stats.probability * Hc;

        Hc = 0.0;

        for (char l : abcCopy) {
            contextStats->nextCharStats[l] = {
                    0,
                    alpha / (contextCount + alpha * abc.size())
            };
        }

        for (char l : lettersNotChanged) {
            stats = &contextStats->nextCharStats[l];
            charCount = stats->count;
            stats->probability = (charCount + alpha) / (contextCount + alpha * abc.size());
        }
    }
}
