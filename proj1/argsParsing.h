
#ifndef ARGUMENTPARSING_H
#define ARGUMENTPARSING_H

#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

namespace argsParsing {

/**
 * Tries to open a file.
 * Exits if it fails.
 *
 * @param fileName name of the file
 * @param type ios_base open mode
 * @param file reference to open the file
 */
void checkAccess(const char *fileName, std::fstream::ios_base::openmode mode,
                 std::fstream &file);

typedef struct {
    int k;
    double alpha;
    std::string initCtx;
    int numChars;
    bool ignorePunctuation = true;
    bool caseSensitive = false;
} ParsingResult;

/**
 * Parses options, number of arguments and gets k and alpha values
 *
 * @param argc main argument
 * @param argv main argument
 * @param HELP help message to display on errs
 * @param numOfArgs number of args (excluding program and options)
 */
ParsingResult parseArguments(int argc, char **argv, const std::string &HELP,
                             int numOfArgs);

}  // namespace argsParsing

#endif  // ARGUMENTPARSING_H
