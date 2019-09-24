
#include <cstring>
#include <fstream>

#include "argsParsing.h"
#include "model.h"

#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    const string HELP =
        "USAGE:\n"
        "   ./generator.cpp [-h] k alpha trainFile outputFile beginSequence "
        "numChars\n"
        "OPTIONS:\n"
        "   h - shows this help\n"
        "   p - don't ignore punctuation\n"
        "   u - case sensitive\n"
        "ARGUMENTS:\n"
        "   k - order  of  the  model\n"
        "   alpha - smoothing  parameter\n"
        "   trainFile - TODO \n"
        "   outputFile - TODO \n"
        "   beginSequence - TODO \n"
        "   numChars TODO \n";

    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 6);

    if (strlen(argv[optind + 4]) != result.k) {
        cerr << "ERROR: beginSequence length must be equal to k" << endl;
        exit(4);
    }
    string initCtx = argv[optind + 4];

    if (argc - 1 != optind + 5) {
        cerr << "ERROR: numChars must be defined" << endl;
        exit(5);
    }
    int numChars = stoi(argv[optind + 5]);

    fstream trainFile;
    argsParsing::checkAccess(argv[optind + 2], fstream::ios_base::in,
                             trainFile);
    fstream outputFile;
    argsParsing::checkAccess(argv[optind + 3], fstream::ios_base::out,
                             outputFile);

    Model m(result.k, result.alpha, &trainFile);

    trainFile.close();

    map<string, map<char, double>> probs = m.getProbs();

    // for (auto &i : probs) {
    //     cout << i.first << endl;
    //     for (auto &f : i.second) {
    //         cout << f.first << ":" << f.second << ",";
    //     }
    //     cout << endl;
    // }

    bool wrote = false;
    char newChar;
    string context = initCtx;
    outputFile << context;
    srand(time(0));
    for (int i = 0; i < numChars; i++) {
        for (auto &tokens : probs) {
            if (!((string)tokens.first).compare(context)) {
                double randNum = ((double)rand() / (RAND_MAX));
                double cumulativeSum = 0.0;
                for (auto &c : tokens.second) {
                    cumulativeSum += c.second;
                    if (cumulativeSum >= randNum) {
                        outputFile << c.first;
                        wrote = true;
                        newChar = c.first;
                        break;
                    }
                }
            }
        }
        if (!wrote) {
            int charIndex =
                (int)(((double)rand() / (RAND_MAX)) * m.getABC().size());
            char c = *std::next(m.getABC().begin(), charIndex);
            outputFile << c;
            newChar = c;
        }
        wrote = false;
        context += newChar;
        context = context.substr(1);
    }

    outputFile.close();

    return 0;
}
