#include <dirent.h>
#include <iostream>
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"

using namespace std;

void parseArguments(int argc, char* argv[],
                    SndfileHandle& sndFileIn, int& blockSize, float& overlapFactor, int& codeBookSize);

int main(int argc, char *argv[]) {
    if (argc < 5) {
        cerr << "Usage: wavfind <input file> <block size> <overlap factor> "
                "<codebook size> <output file>"
             << endl;
        return 1;
    }

    // parse and validate arguments
    SndfileHandle sndFileIn{argv[argc - 5]};
    int blockSize, codebookSize;
    float overlapFactor;
    parseArguments(argc, argv,
                   sndFileIn, blockSize, overlapFactor, codebookSize);

    //retrieve all blocks
    vector<vector<short>> blocks;
    vector<short> block(blockSize * sndFileIn.channels());
    int i = 0;
    while (sndFileIn.readf(block.data(), blockSize)) {
        blocks.push_back(block);
        //cout << blocks.at(i).at(0) << endl;
        sndFileIn.seek((blockSize - (int)blockSize * overlapFactor) * i,
                       SEEK_SET);

        i++;
    }

    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    dp = opendir(cbFolder);
    if (dp != nullptr) {
        while ((entry = readdir(dp))) printf("%s\n", entry->d_name);
    }

    closedir(dp);

    return 0;
}

void parseArguments(int argc, char* argv[],
                    SndfileHandle& sndFileIn, int& blockSize, float& overlapFactor, int& codebookSize) {

    checkFileToRead(sndFileIn, argv[argc - 5]);
    int sndFileSize = sndFileIn.frames();

    // validate block size
    try {
        blockSize = stoi(argv[argc - 4]);
    } catch (...) {
        cerr << "Error: block size must be a valid number" << endl;
        exit(1);
    }
    if (blockSize <= 0) {
        cerr << "Error: block size must be larger than zero" << endl;
        exit(1);
    }
    if (blockSize > sndFileSize) {
        cerr << "Error: block size must be smaller than " << sndFileSize
             << endl;
        exit(1);
    }

    // validate overlap factor
    float overlap_factor;
    try {
        overlap_factor = stof(argv[argc - 3]);
    } catch (...) {
        cerr << "Error: overlap factor must be a valid number" << endl;
        exit(1);
    }
    if (overlap_factor < 0 || overlap_factor > 1) {
        cerr << "Error: overlap factor must be a number between 0 and 1"
             << endl;
        exit(1);
    }

    // validate codebook size
    try {
        codebookSize = stoi(argv[argc - 2]);
    } catch (...) {
        cerr << "Error: codebook size must be a valid number" << endl;
        exit(1);
    }
    if (codebookSize <= 0) {
        cerr << "Error: codebook size must be larger than zero" << endl;
        exit(1);
    }
}
