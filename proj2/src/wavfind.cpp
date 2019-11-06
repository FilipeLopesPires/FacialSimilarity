#include <dirent.h>
#include <cstring>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"
#include "headers/vctQuant.h"

using namespace std;
namespace fs = experimental::filesystem;

void parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn,
                    int& blockSize, float& overlap_factor, string& cbDir) {
    checkFileToRead(sndFileIn, argv[argc - 4]);
    int sndFileSize = sndFileIn.frames();

    // validate block size
    try {
        blockSize = stoi(argv[argc - 3]);
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
    try {
        overlap_factor = stof(argv[argc - 2]);
    } catch (...) {
        cerr << "Error: overlap factor must be a valid number" << endl;
        exit(1);
    }
    if (overlap_factor < 0 || overlap_factor > 1) {
        cerr << "Error: overlap factor must be a number between 0 and 1"
             << endl;
        exit(1);
    }

    cbDir = argv[argc - 1];
}

long calculateError(vector<vector<short>>& blocks,
                    vector<vector<short>>& codeBook) {
    long error{};
    int idx{0};
    for (auto& block : blocks) {
        for (auto& cb : codeBook) {
            long er = calcEn(block, cb);
            if (idx == 0) {
                error = er;
            } else {
                if (er < error) {
                    error = er;
                }
            }
            idx++;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: wavfind <input file> <block size> <overlap "
                "factor> <codebook directory>"
             << endl;
        return 1;
    }

    // parse and validate arguments
    SndfileHandle sndFileIn{argv[argc - 4]};
    int blockSize;
    float overlapFactor;
    string cbDir{};
    parseArguments(argc, argv, sndFileIn, blockSize, overlapFactor, cbDir);

    // retrieve all blocks
    vector<vector<short>> blocks;
    retrieveBlocks(blocks, sndFileIn, blockSize, overlapFactor);

    // fs::path cbPath{cbDir.c_str()};
    // for (auto& entry : fs::directory_iterator(cbPath)) {
    //     if (fs::is_regular_file(entry.status())) {
    //         cout << entry.path().filename() << endl;
    //     } else {
    //         cerr << "Error: invalid file type of file "
    //              << entry.path().filename() << endl;
    //     }
    // }

    struct dirent* entry = nullptr;
    DIR* dp = nullptr;

    long minimum{};
    map<string, int> errors{};
    vector<vector<short>> codeBook;
    dp = opendir(cbDir.c_str());
    if (dp != nullptr) {
        while (entry = readdir(dp)) {
            if (entry->d_type == 8) {
                string name = entry->d_name;
                ifstream file(cbDir + "/" + name);
                string line;
                while (getline(file, line)) {
                    vector<short> aux{};
                    size_t pos = 0;
                    while ((pos = line.find(",")) != std::string::npos) {
                        aux.push_back(short(stoi(line.substr(0, pos))));
                        line.erase(0, pos + string(",").length());
                    }
                    codeBook.push_back(aux);
                }
                // get error from blocks vs codeBook
                long er = calculateError(blocks, codeBook);
                minimum = er;
                errors.insert({entry->d_name, er});
                codeBook.clear();
            }
        }
    }

    string response{};
    for (auto& pair : errors) {
        if (pair.second < minimum) {
            response = pair.first;
            minimum = pair.second;
        }
    }

    closedir(dp);

    cout << response << endl;

    return 0;
}