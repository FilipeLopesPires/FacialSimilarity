#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iterator>

#include <cstdlib> // strtoul, strtod
#include <getopt.h>

#include <cmath>

class Model {
    private:
        int ctxLen;                             // context length
        double alpha;                           // smoothing paramenter 
        std::ifstream reader;
  		std::set<char> abc;													// alphabet of the test data
        std::map<std::string,std::map<char,int>> occurTable;   // table with number of occurrences of each letter after each context present on the test data {context:{letter:num_occur}} 

    public:
        Model() {
            ctxLen = 1;
            alpha = 1.0;
            reader.open("text.txt", std::ifstream::in);
        }

        Model(int ctxLen, double alpha) {
            this -> ctxLen = ctxLen;
            this -> alpha = alpha;
        }

        ~Model() {}

        int getCtxLen() { return ctxLen; }
        double getAlpha() { return alpha; }
        std::map<std::string,std::map<char,int>> getOccurTable(){ return occurTable; }

    void fileParser(){
        std::map<std::string, std::map<char, int>> relFreq;
        char letter;
        std::string context;

        while(reader.get(letter)){
            if(abc.find(letter) == abc.end()){
                abc.insert(letter);
            }
            if(context.length()>=ctxLen){
                if(relFreq.find(context) != relFreq.end()){
                    std::map<char, int> nextChar={{letter,1}};
                    relFreq.insert(std::pair<std::string, std::map<char, int>>(context, nextChar));
                }else{
                    if(relFreq[context].find(letter) == relFreq[context].end()){
                        relFreq[context].insert(std::pair<char,int>(letter,1));
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

    double getModelEntropy() const {
        //string ctx;
        std::map<char,int> occurMap;
        
        //char curChar;
        int curOccur;
        int totalOccur = 0;
        
        double ctxProb = 0.0;
        double conditionalProb;
        
        double H = 0.0;
        double Hc = 0.0;

        for(auto it : occurTable) {
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
};

int main(){
    Model m;
    m.fileParser();
    auto aux = m.getOccurTable();
    std::cout << m.getModelEntropy() << std::endl;
    return 0;

}