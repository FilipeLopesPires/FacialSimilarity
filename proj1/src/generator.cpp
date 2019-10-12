
#include <fstream>
#include <random>

#include "argsParsing.h"
#include "model.h"

using namespace std;

int main(int argc, char **argv) {
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

    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 6);

    Model m(result.k, result.alpha);

    m.parseFile(result.inputFiles);

    for (auto trainFile : result.inputFiles) {
        trainFile->close();
    }

    // get the filled statsTable
    auto probs = m.getStatsTable();

    // to know if after finding a known context a character was
    // written to the output file
    bool wrote = false;
    // new character of the context/sliding window
    char newChar;
    // current context/sliding window
    string context = result.initCtx;
    // write to the output file the initial context
    result.outputFile << context;

    default_random_engine eng((random_device())());
    // used to generate a random probability
    uniform_real_distribution<double> prob_rand(0, 1);
    // used to generate a random character of the alphabet
    uniform_int_distribution<int> idx_rand(0, m.getABC().size() - 1);

    for (int i = 0; i < result.numChars; i++) {
        // if the stats table contains the current context
        if (probs.count(context)) {
            // generate a random probability number
            double randNum = prob_rand(eng);
            double cumulativeSum = 0.0;

            // iterate over the characters' stats after the current context
            for (auto &c : probs.at(context).nextCharStats) {
                // and keep adding their probability
                cumulativeSum += c.second.probability;

                // until it exceeds the random probability generated
                if (cumulativeSum >= randNum) {
                    // them write to the output file that character
                    result.outputFile << c.first;
                    wrote = true;
                    newChar = c.first;
                    break;
                }
            }
        }

        // else if the current context is unknown
        if (!wrote) {
            // choose a random character form the alphabet
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
