
#include "argsParsing.h"

using namespace std;
using namespace argsParsing;

// checks access to the files given as arguments
void argsParsing::checkAccess(const char *fileName,
                              fstream::ios_base::openmode mode, fstream &file) {
    file.open(fileName, mode);

    // throws error if unable to open file
    if (!file.is_open()) {
        cerr << "Couldn't open file " << fileName << endl;
        exit(3);
    }
}

// checks if arguments are all in the correct order and format
ParsingResult argsParsing::parseArguments(int argc, char **argv,
                                          const string &HELP,
                                          int numOfArgs) {
    ParsingResult result;

    // parse options
    int c = 0;
    while ((c = getopt(argc, argv, "hpc")) != -1) {
        switch (c) {
            case 'h':
                cout << HELP << endl;
                exit(0);
            /*case 'p':
                result.ignorePunctuation = false;
                break;
            case 'c':
                result.caseSensitive = true;
                break;*/
            default:
                exit(1);
        }
    }

    // -- parse arguments --
    if (argc - optind < numOfArgs) {
        cerr << "ERROR: Missing parameters" << endl << HELP << endl;
        exit(2);
    }

    int argIdx = optind;

    // parse k
    try {
        result.k = stoi(argv[argIdx]);
    } catch (...) {
        cerr << "k should be a positive integer" << endl;
        exit(3);
    }
    if (result.k <= 0) {
        cerr << "k should be greater than 0" << endl;
        exit(3);
    }
    argIdx++;

    // parse alpha
    try {
        result.alpha = stod(argv[argIdx], nullptr);
    } catch (...) {
        cerr << "alpha should be an floating point number" << endl;
        exit(3);
    }

    // parse trainFiles
    for (int idx = ++argIdx; idx < argc; idx++) {
        fstream *trainFile = new fstream();
        checkAccess(argv[idx], fstream::ios_base::in, *trainFile);
        result.inputFiles.push_back(trainFile);
    }

    return result;
}
