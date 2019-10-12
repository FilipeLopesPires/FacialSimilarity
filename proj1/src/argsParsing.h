
#ifndef ARGUMENTPARSING_H
#define ARGUMENTPARSING_H

#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

/*!
 * Functions and structures related to the parsing of the options and
 *  arguments of a program.
 */
namespace argsParsing {

/**
 * Tries to open a file.
 * Exits if it fails.
 *
 * @param fileName name of the file
 * @param mode ios_base open mode
 * @param file reference to open the file
 */
void checkAccess(const char *fileName, std::fstream::ios_base::openmode mode,
                 std::fstream &file);

/*!
 * Structure to hold all data read and parsed from program options and arguments
 */
typedef struct {
    /*!
     * Size of the sliding window of the model (fcm and generator)
     */
    int k;

    /*!
     * Smoothing parameter (fcm and generator)
     */
    double alpha;

    /*!
     * Files to feed to the model as training data (fcm and generator)
     */
    std::list<std::fstream*> inputFiles;

    /*!
     * File to write the generated text from the generator (generator only)
     */
    std::fstream outputFile;

    /*!
     * First sequence of the generated text of the generator (generator only)
     */
    std::string initCtx;

    /*!
     * Number of character to generate (generator only)
     */
    int numChars;

    /*!
     * If the model will ignore punctuation (default: true)
     */
    //bool ignorePunctuation = true;

    /*!
     * If the model is case sensitive (default: false)
     */
    //bool caseSensitive = false;
} ParsingResult;

/*!
 * Parses options, number of arguments and gets k and alpha values
 *
 * @param argc main argument
 * @param argv main argument
 * @param HELP help message to display on errs
 * @param numOfArgs number of args (excluding program and options)
 * @return struct with parsed arguments
 */
ParsingResult parseArguments(int argc, char **argv, const std::string &HELP,
                             int numOfArgs);

}  // namespace argsParsing

#endif  // ARGUMENTPARSING_H
