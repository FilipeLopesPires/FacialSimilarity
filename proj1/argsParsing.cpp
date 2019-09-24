
#include "argsParsing.h"

using namespace std;

void argsParsing::checkAccess(const char *fileName,
                              fstream::ios_base::openmode mode, fstream &file) {
    file.open(fileName, mode);

    if (!file.is_open()) {
        cerr << "Couldn't open file " << fileName << endl;
        exit(3);
    }
}

argsParsing::ParsingResult argsParsing::parseArguments(int argc, char **argv,
                                                       const string &HELP,
                                                       int numOfArgs) {
    ParsingResult result;

    // parse options
    int c = 0;
    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
            case 'h':
                cout << HELP << endl;
                exit(0);
            case 'p':
                result.ignorePunctuation = false;
                break;
            case 'c':
                result.caseSensitive = true;
                break;
            default:
                cerr << "ERROR: Unknown " << c << " option" << endl;
                exit(1);
        }
    }

    // parse arguments
    if (argc - optind != numOfArgs) {
        cerr << "ERROR: Missing parameters" << endl << HELP << endl;
        exit(2);
    }

    int argIdx = optind;

    errno = 0;
    result.k = strtoul(argv[argIdx], nullptr, 0);
    if (errno) {
        cerr << "k should be a positive integer" << endl;
        exit(3);
    }
    argIdx++;

    errno = 0;
    result.alpha = strtod(argv[argIdx], nullptr);
    if (errno) {
        cerr << "alpha should be an floating point number" << endl;
        exit(3);
    }

    return result;
}
