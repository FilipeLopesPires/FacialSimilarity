
#include <iostream>

#include "argsParsing.h"
#include "model.h"

using namespace std;

// Main function. Run this file to generate the Markov's model and calculate its entropy.
int main(int argc, char **argv) {

    // HELP string used to aid an unexperienced user when executing fcm
    const string HELP =
        "USAGE:\n"
        "   ./fcm [-h] k alpha trainFile [trainFile ...]\n"
        "OPTIONS:\n"
        "   h - shows this help\n"
        /*"   p - don't ignore punctuation\n"
        "   u - case sensitive\n"*/
        "ARGUMENTS:\n"
        "   k - order  of  the  model\n"
        "   alpha - smoothing  parameter\n"
        "   trainFile - path to a file to feed to the model as training data";

    // arguments validation
    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 3);

    // model generation
    model::Model m(result.k, result.alpha);
    m.parseFile(result.inputFiles);

    for (auto trainFile : result.inputFiles) {
        trainFile->close();
    }

    // print of the calculated value for the model's entropy 
    cout << "Model Entropy: " << m.getModelEntropy() << endl;

    return 0;
}
