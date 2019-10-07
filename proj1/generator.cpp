
#include <fstream>
#include <random>

#include "argsParsing.h"
#include "model.h"

using namespace std;

int main(int argc, char **argv) {
    const string HELP =
        "USAGE:\n"
        "   ./generator.cpp [-h] k alpha beginSequence numChars outputFile "
        "trainFile [trainFile ...]"
        "numChars\n"
        "OPTIONS:\n"
        "   h - shows this help\n"
        /*"   p - don't ignore punctuation\n"
        "   u - case sensitive\n"*/
        "ARGUMENTS:\n"
        "   k - order  of  the  model\n"
        "   alpha - smoothing  parameter\n"
        "   trainFile - TODO \n"
        "   outputFile - TODO \n"
        "   beginSequence - TODO \n"
        "   numChars TODO \n";

    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 6);

    Model m(result.k, result.alpha);

    m.parseFile(result.inputFiles);

    for (auto trainFile : result.inputFiles) {
        trainFile->close();
    }

    auto probs = m.getStatsTable();

    bool wrote = false;
    char newChar;
    string context = result.initCtx;
    result.outputFile << context;

    default_random_engine eng((random_device())());
    uniform_real_distribution<double> prob_rand(0, 1);
    uniform_int_distribution<int> idx_rand(0, m.getABC().size() - 1);

    for (int i = 0; i < result.numChars; i++) {
        if (probs.count(context)) {
            double randNum = prob_rand(eng);
            double cumulativeSum = 0.0;
            for (auto &c : probs.at(context).nextCharStats) {
                cumulativeSum += c.second.probability;
                if (cumulativeSum >= randNum) {
                    result.outputFile << c.first;
                    wrote = true;
                    newChar = c.first;
                    break;
                }
            }
        }
        if (!wrote) {
            int charIndex = idx_rand(eng);
            char c = *std::next(m.getABC().begin(), charIndex);
            result.outputFile << c;
            newChar = c;
        }
        wrote = false;
        context += newChar;
        context = context.substr(1);
    }

    result.outputFile.close();

    return 0;
}
