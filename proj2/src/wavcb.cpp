
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sndfile.hh>
#include <vector>

#include "headers/io.h"
#include "headers/vctQuant.h"

#define DEBUG 0

using namespace std;

void parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn,
                    int& blockSize, float& overlapFactor, float& errorThreshold,
                    string& outputFile);

void writeCentroids(string& filename, vector<vector<short>>& centroids);

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: wavcb <input file> <block size> <overlap factor> "
                "<error threshold> <output file>"
             << endl;
        return 1;
    }

    // parse and validate arguments
    SndfileHandle sndFileIn{argv[argc - 5]};
    int blockSize, codebookSize;
    float overlapFactor, errorThreshold;
    string outputFile;
    parseArguments(argc, argv, sndFileIn, blockSize, overlapFactor,
                   errorThreshold, outputFile);

    // retrieve all blocks
    vector<vector<short>> blocks;
    retrieveBlocks(blocks, sndFileIn, blockSize, overlapFactor);

    cout << errorThreshold << endl;
    codebookSize = (int)(log2(blocks.size()));
    cout << blocks.size() << " || " << codebookSize << endl;
    // validate number of centroids
    if (blocks.size() < codebookSize) {
        cerr << "Error: too many centroids for the given initial arguments "
                "(maximum = " +
                    to_string(blocks.size()) + ")"
             << endl;
        return 1;
    }

    // initialize centroids
    vector<vector<short>> centroids;
    {
        set<int> indexesUsed;
        int index, prev_centroidvec_size;
        for (int i = 0; i < codebookSize; i++) {
            prev_centroidvec_size = centroids.size();
            while (centroids.size() == prev_centroidvec_size) {
                index = rand() % blocks.size();
                if (indexesUsed.count(index) == 0) {
                    centroids.emplace_back(blocks.at(index));
                    indexesUsed.insert(index);
                }
            }
        }
    }

    double lastError = 0, currentError = 0;
    // apply k-means algorithm (optimize centroids)
    do {  // TODO !maxIterationsReached || !errorLowerThanThreshold ->
          // OPTIONAL program options
        // calculate closest blocks for each centroid
        lastError = currentError;
        currentError = 0;
        vector<vector<vector<short>*>> closest_blocks(centroids.size());
        long error, smallest_local_error;
        int local_centroid_idx;
        for (auto& block : blocks) {
            smallest_local_error = calcEn(block, centroids.at(0));
            local_centroid_idx = 0;
            for (size_t j = 1; j < centroids.size(); j++) {
                error = calcEn(block, centroids.at(j));
                if (error < smallest_local_error) {
                    smallest_local_error = error;
                    local_centroid_idx = j;
                }
            }
            currentError += smallest_local_error;
            closest_blocks.at(local_centroid_idx).push_back(&block);
        }

        // for (auto& b1 : closest_blocks) {
        //     for (auto& b2 : b1) {
        //         cout << "alperce"
        //              << ",";
        //     }
        //     cout << endl;
        // }

        // cout << "damasqueiro" << endl;

        // update centroids

        for (size_t i = 0; i < centroids.size(); i++) {
            if (closest_blocks[i].empty()) {
                continue;
            } else if (closest_blocks[i].size() == 1) {
                for (size_t idx = 0; idx < blockSize; idx++) {
                    centroids[i][idx] = closest_blocks[i][0]->at(idx);
                }
            }

            vector<double> sums_blocks(blockSize);
            for (auto& block : closest_blocks[i]) {
                for (size_t j = 0; j < block->size(); j++) {
                    sums_blocks[j] += block->at(j);
                }
            }
            for (int idx = 0; idx < blockSize; idx++) {
                centroids[i][idx] = sums_blocks[idx] / closest_blocks[i].size();
            }
        }
        if (lastError == 0) {
            lastError = 2 * currentError;
        }
        cout << ((lastError - currentError) / lastError) << endl;
    } while (((lastError - currentError) / lastError) > errorThreshold);

    // TODO deal with isolated centroids

    writeCentroids(outputFile, centroids);

    return 0;
}

#if DEBUG
void writeCentroids(string& filename, vector<vector<short>>& centroids) {
    ofstream file(filename);

    for (size_t i = 0; i < centroids.size(); i++) {
        vector<short>& centroid = centroids[i];
        for (size_t j = 0; j < centroid.size(); j++) {
            file << centroid[j];

            if (j < centroid.size() - 1) {
                file << ',';
            }
        }

        if (i < centroids.size()) {
            file << endl;
        }
    }

    file.close();
}
#else
void writeCentroids(string& filename, vector<vector<short>>& centroids) {
    ofstream file(filename, fstream::binary);

    size_t blockSize = centroids[0].size();
    file.write((char*)&blockSize, sizeof(size_t));
    size_t numOfCentroids = centroids.size();
    file.write((char*)&numOfCentroids, sizeof(size_t));

    for (auto& centroid : centroids) {
        for (short frame : centroid) {
            file.write((char*)&frame, sizeof(short));
        }
    }

    file.close();
}
#endif

void parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn,
                    int& blockSize, float& overlapFactor, float& errorThreshold,
                    string& outputFile) {
    // validate input file
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
    try {
        overlapFactor = stof(argv[argc - 3]);
    } catch (...) {
        cerr << "Error: overlap factor must be a valid number" << endl;
        exit(1);
    }
    if (overlapFactor < 0 || overlapFactor > 1) {
        cerr << "Error: overlap factor must be a number between 0 and 1"
             << endl;
        exit(1);
    }

    // validate codebook size
    try {
        errorThreshold = stof(argv[argc - 2]);
    } catch (...) {
        cerr << "Error: errorThreshold must be a valid number" << endl;
        exit(1);
    }
    if (errorThreshold <= 0) {
        cerr << "Error: errorThreshold must be larger than zero" << endl;
        exit(1);
    }

    outputFile = argv[argc - 1];
}
