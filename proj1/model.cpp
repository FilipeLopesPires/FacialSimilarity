
#include "model.h"

using namespace std;

void Model::fileParser(){
    map<string, map<char, int>> relFreq;
    char letter;
    string context;

    while(reader.get(letter)){
        if(abc.find(letter) == abc.end()){
            abc.insert(letter);
        }
        if(context.length()>=ctxLen){
            if(relFreq.find(context) != relFreq.end()){
                map<char, int> nextChar={{letter,1}};
                relFreq.insert(pair<string, map<char, int>>(context, nextChar));
            }else{
                if(relFreq[context].find(letter) == relFreq[context].end()){
                    relFreq[context].insert(pair<char,int>(letter,1));
                }else{
                    relFreq[context][letter]=relFreq[context][letter]+1;
                }
            }
            context=context.substr(1,2);
            context+=letter;
        }else{
            context+=letter;
        }
    }
    for(auto &pair : relFreq){
        if(pair.second.empty()){
            relFreq.erase(pair.first);
        }
    }
    occurTable=relFreq;
}

double Model::getModelEntropy() const {
    //string ctx;
    map<char,int> occurMap;

    //char curChar;
    int curOccur;
    int totalOccur = 0;

    double ctxProb = 0.0;
    double conditionalProb;

    double H = 0.0;
    double Hc = 0.0;

    for(const auto it : occurTable) {
        occurMap = it.second;

        for(auto it2 :occurMap ) { totalOccur += it2.second; }

        for(auto it2 :occurMap ) {
            curOccur = it2.second;
            conditionalProb = (curOccur + alpha) / (totalOccur + alpha*abc.size());
            ctxProb += conditionalProb;
            Hc += (conditionalProb * log2(conditionalProb));
        }
        H += -(ctxProb * Hc);
        Hc = 0.0;
        ctxProb = 0.0;
    }

    return H;
}
