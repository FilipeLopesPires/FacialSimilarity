
#include "model.h"
#include <iostream>

using namespace std;

void Model::fileParser() {
    map<string, map<char, int>> relFreq;
    char letter;
    string context;

    while (reader->get(letter)) {
        if (abc.find(letter) == abc.end()) {
            abc.insert(letter);
        }
        if (context.length() >= ctxLen) {
            if (relFreq[context].find(letter) == relFreq[context].end()) {
                relFreq[context].insert(pair<char, int>(letter, 1));
            } else {
                relFreq[context][letter] = relFreq[context][letter] + 1;
            }
            context = context.substr(1);
        }
        context += letter;
    }
    for (auto &pair : relFreq) {
        if (pair.second.empty()) {
            relFreq.erase(pair.first);
        }
    }
    occurTable = relFreq;
}

double Model::getModelEntropy() const {
    // string ctx;
    string context;
    map<char, int> occurMap;

    int curOccur;

    int contextCountTotal = 0;
    map<string, int> contextCount;

    double conditionalProb;

    double H = 0.0;
    double Hc = 0.0;

    for (auto &it : occurTable) {
        context = it.first;
        occurMap = it.second;

        for (auto &it2 : occurMap) {
            contextCount[context] += it2.second;
            contextCountTotal += it2.second;
        }
    }

    for (auto &it : occurTable) {
        context = it.first;
        occurMap = it.second;

        for (auto &it2 : occurMap) {
            curOccur = it2.second;
            conditionalProb = (curOccur + alpha) /
                              (contextCount[context] + alpha * abc.size());
            Hc += (conditionalProb * log2(conditionalProb));
        }
        H += -(((double)contextCount[context] / contextCountTotal) * Hc);
        Hc = 0.0;
    }

    return H;
}

map<string, map<char, double>> Model::getProbs() {
    string context;
    map<char, int> occurMap;
    map<string, map<char, double>> probs;

    int curOccur;

    int contextCountTotal = 0;
    map<string, int> contextCount;

    double conditionalProb;

    for (auto &it : occurTable) {
        context = it.first;
        occurMap = it.second;

        for (auto &it2 : occurMap) {
            contextCount[context] += it2.second;
        }
    }

    for (auto &it : occurTable) {
        context = it.first;
        occurMap = it.second;

        set<char> abcCopy(abc);

        for (auto &it2 : occurMap) {
            curOccur = it2.second;
            conditionalProb = (curOccur + alpha) /
                              (contextCount[context] + alpha * abc.size());
            probs[context][it2.first] = conditionalProb;
            abcCopy.erase(it2.first);
        }

        if (abcCopy.size() != 0) {
            for (auto &c : abcCopy) {
                conditionalProb =
                    (alpha) / (contextCount[context] + alpha * abc.size());
                probs[context][c] = conditionalProb;
            }
        }
    }
    return probs;
}
