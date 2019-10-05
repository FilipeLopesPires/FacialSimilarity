
#include <fstream>
#include <iostream>

#include "argsParsing.h"
#include "model.h"

using namespace std;

int main(int argc, char **argv) {
    const string HELP =
        "USAGE:\n"
        "   ./fcm.cpp [-h] k alpha trainFile\n"
        "OPTIONS:\n"
        "   h - shows this help\n"
        "   p - don't ignore punctuation\n"
        "   u - case sensitive\n"
        "ARGUMENTS:\n"
        "   k - order  of  the  model\n"
        "   alpha - smoothing  parameter\n"
        "   trainFile - TODO";

    argsParsing::ParsingResult result =
        argsParsing::parseArguments(argc, argv, HELP, 3);

    // fstream trainFile;
    // argsParsing::checkAccess(argv[optind + 2], fstream::ios_base::in,
    //                          trainFile);

    Model m(result.k, result.alpha);
    m.parseFile(result.inputFiles);
    trainFile.close();

    cout << "Model Entropy: " << m.getModelEntropy() << endl;

    return 0;
}
