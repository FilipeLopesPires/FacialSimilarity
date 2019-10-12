
#include <fstream>
#include <random>

#include "argsParsing.h"
#include "model.h"

using namespace std;

// Main function. Run this file to generate text automatically following a Markov's model built from the input files passed as arguments
int main(int argc, char **argv) {

    // HELP string used to aid an unexperienced user when executing generator
    const string HELP =
        "USAGE:\n"
        "   ./generator [-h] k alpha beginSequence numChars outputFile "
        "trainFile [trainFile ...]"
        "OPTIONS:\n"
        "   h - shows this help\n"
        /*"   p - don't ignore punctuation\n"
        "   u - case sensitive\n"*/
        "ARGUMENTS:\n"
        "   k - order  of  the  model\n"
        "   alpha - smoothing  parameter\n"
        "   beginSequence - First character sequence of the text generated \n"
        "   numChars - number of characters to generate \n"
        "   outputFile - path to file where the generator output will be stored \n"
        "   trainFile - path to a file to feed to the model as training data";

    // arguments validation
    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 6);

    // model generation
    Model m(result.k, result.alpha);
    m.parseFile(result.inputFiles);

    for (auto trainFile : result.inputFiles) {
        trainFile->close();
    }

    // generate text starting from the 'beginSequence' and writting 'numChars' characters to an 'outputFile'

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
