
#include <cstring>
#include <fstream>

#include "argsParsing.h"

using namespace std;

int main2(int argc, char **argv) {
    const string HELP =
            "USAGE:\n"
            "   ./generator.cpp [-h] k alpha trainFile outputFile beginSequence\n"
            "OPTIONS:\n"
            "   h - shows this help\n"
            "   p - don't ignore punctuation\n"
            "   u - case sensitive\n"
            "ARGUMENTS:\n"
            "   k - order  of  the  model\n"
            "   alpha - smoothing  parameter\n"
            "   trainFile - TODO \n"
            "   outputFile - TODO \n"
            "   beginSequence - TODO \n";

    argsParsing::ParsingResult result = argsParsing::parseArguments(argc, argv, HELP, 5);

    if (strlen(argv[optind + 4]) != result.k) {
        cerr << "ERROR: beginSequence length must be equal to k" << endl;
        exit(4);
    }

    fstream trainFile;
    argsParsing::checkAccess(argv[optind + 2], fstream::ios_base::in, trainFile);
    fstream outputFile;
    argsParsing::checkAccess(argv[optind + 3], fstream::ios_base::out, outputFile);

    // do stuff

    trainFile.close();

    // do more stuff

    outputFile.close();

    return 0;
}
