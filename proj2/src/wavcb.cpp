
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <semaphore.h>
#include <set>
#include <sndfile.hh>
#include <queue>
#include <vector>
#include <thread>

#include "headers/io.h"
#include "headers/vctQuant.h"

#define DEBUG 0

using namespace std;


void parseArguments(int argc, char* argv[], SndfileHandle& sndFileIn,
                    int& blockSize, float& overlapFactor, float& errorThreshold,
                    int& numRuns, string& outputFile, int& numThreadsForRuns,
                    int& numThreadsForEachRun);

void writeCentroids(string& filename, vector<vector<short>>& centroids);

typedef struct {
    vector<vector<short>>* data;
    double error;
} Centroids;

void applyKMeans(
        int numCentroids,
        vector<vector<short>>& blocks,
        int threadsPerRun,
        int blockSize,
        float errorThreshold,
        mutex& centroidsToCmpMutex,
        queue<Centroids>& centroidsToCompare,
        sem_t* centroidsToCmpAvailable
);

void calculateClosestBlocks(
        int blocksBeginIdx,
        int blocksEndIdx,
        vector<vector<short>>& blocks,
        vector<vector<short>>* centroids,
        int numCentroids,
        vector<vector<vector<short>*>>& closestBlocksPerCentroid
);


int main(int argc, char* argv[]) {
    if (argc != 7 && argc != 9) {
        cerr << "Usage: wavcb <inputFile> <blockSize> <overlapFactor> "
                "<errorThreshold> <numRuns> <output file> [<threads for runs> <threads per run>]"
             << endl;
        return 1;
    }

    // parse and validate arguments
    SndfileHandle sndFileIn{argv[argc - 6 - (argc == 9 ? 2 : 0)]};
    int blockSize, codeBookSize, numRuns, numThreadsForRuns, numThreadsForEachRun;
    float overlapFactor, errorThreshold;
    string outputFile;
    parseArguments(argc, argv, sndFileIn, blockSize, overlapFactor,
                   errorThreshold, numRuns, outputFile, numThreadsForRuns, numThreadsForEachRun);

    // retrieve all blocks
    vector<vector<short>> blocks;
    retrieveBlocks(blocks, sndFileIn, blockSize, overlapFactor);

    codeBookSize = (int)(blocks.size() / 4);

    // validate number of centroids
    if (blocks.size() < codeBookSize) {
        cerr << "Error: too many centroids for the given initial arguments "
                "(maximum = " <<
                blocks.size() << ")"
             << endl;
        return 1;
    }

    Centroids bestCentroids = {new vector<vector<short>>(0), numeric_limits<double>::max()};
    // ^^ the allocation of the empty vector is to
    // make sure that first free on bestCentroids doesn't raise an error
    Centroids centroidsComparing;

    sem_t centroidsToCmpAvailable;
    sem_init(&centroidsToCmpAvailable, 0, 0);
    queue<Centroids> centroidsToCompare;
    mutex centroidsToCmpMutex;
    thread t;
    for (int i = 0; i < numThreadsForRuns; i++) {
        t = thread(
                applyKMeans,
                codeBookSize,
                std::ref(blocks),
                numThreadsForEachRun,
                blockSize,
                errorThreshold,
                std::ref(centroidsToCmpMutex),
                std::ref(centroidsToCompare),
                &centroidsToCmpAvailable
                );
        t.detach();
    }

    while (numRuns > 0) {
        sem_wait(&centroidsToCmpAvailable);

        if (--numRuns > 0) {
            t = thread(
                    applyKMeans,
                    codeBookSize,
                    std::ref(blocks),
                    numThreadsForEachRun,
                    blockSize,
                    errorThreshold,
                    std::ref(centroidsToCmpMutex),
                    std::ref(centroidsToCompare),
                    &centroidsToCmpAvailable
            );
            t.detach();
        }

        centroidsToCmpMutex.lock();
        centroidsComparing = centroidsToCompare.front();
        cout << "solution found with error " << centroidsComparing.error << endl;
        if (centroidsComparing.error < bestCentroids.error) {
            free(bestCentroids.data);
            bestCentroids = centroidsComparing;
        }
        else {
            free(centroidsComparing.data);
        }
        centroidsToCompare.pop();
        centroidsToCmpMutex.unlock();
    }

    cout << "best centroids with error " << bestCentroids.error << endl;

    writeCentroids(outputFile, *bestCentroids.data);

    return 0;
}

void applyKMeans(
        int numCentroids,
        vector<vector<short>>& blocks,
        int threadsPerRun,
        int blockSize,
        float errorThreshold,
        mutex& centroidsToCmpMutex,
        queue<Centroids>& centroidsToCompare,
        sem_t* centroidsToCmpAvailable
) {

    // initialize centroids
    vector<vector<short>>* centroids = new vector<vector<short>>();
    {
        set<size_t> indexesUsed;
        int index, prevCentroidsSize;
        for (int i = 0; i < numCentroids; i++) {
            prevCentroidsSize = centroids->size();
            while (centroids->size() == prevCentroidsSize) {
                index = rand() % blocks.size();
                if (indexesUsed.count(index) == 0) {
                    centroids->emplace_back(blocks[index]);
                    indexesUsed.insert(index);
                }
            }
        }
    }

    int numBlocksPerThread = ceil(blocks.size() / (double) threadsPerRun);
    vector<vector<vector<vector<short>*>>> closestBlocksPerCentroidPerThread(threadsPerRun);
    for (int i = 0; i < threadsPerRun; i++) {
        for (int j = 0; j < numCentroids; j++) {
            closestBlocksPerCentroidPerThread[i].emplace_back();
        }
    }
    thread threads[threadsPerRun];

    double lastError, currentError = numeric_limits<double>::max(), diff;
    do {
        lastError = currentError;

        for (auto& closestBlocksPerCentroid : closestBlocksPerCentroidPerThread) {
            for (vector<vector<short>*>& closestBlocksForCentroid : closestBlocksPerCentroid) {
                closestBlocksForCentroid.clear();
            }
        }

        int begin, end = 0;
        for (int i = 0; i < threadsPerRun; i++) {
            begin = end;

            if (i == threadsPerRun - 1) {
                end = blocks.size();
            }
            else {
                end = begin + numBlocksPerThread;
            }

            threads[i] = thread(
                    calculateClosestBlocks,
                    begin,
                    end,
                    std::ref(blocks),
                    centroids,
                    numCentroids,
                    std::ref(closestBlocksPerCentroidPerThread[i])
            );
        }

        for (int i = 0; i < threadsPerRun; i++) {
            threads[i].join();
        }

        // update centroids
        double sums_blocks[blockSize];
        for(size_t i = 0; i < numCentroids; i++) {
            size_t closestBlocksCount = 0;
            for (size_t j = 0; j < blockSize; j++) {
                sums_blocks[j] = 0;
            }

            for (size_t threadIdx = 0; threadIdx < threadsPerRun; threadIdx++) {
                for (vector<short>* block : closestBlocksPerCentroidPerThread[threadIdx][i]) {
                    closestBlocksCount++;

                    for (size_t blockIdx = 0; blockIdx < blockSize; blockIdx++) {
                        sums_blocks[blockIdx] += block->at(blockIdx);
                    }
                }
            }

            if (closestBlocksCount == 1) {
                for (size_t idx = 0; idx < blockSize; idx++) {
                    centroids->at(i)[idx] = sums_blocks[idx];
                }
            }
            else if (closestBlocksCount > 1) {
                for(int idx=0; idx<blockSize;idx++){
                    centroids->at(i)[idx] = sums_blocks[idx] / closestBlocksCount;
                }
            }
        }

        currentError = calculateError(blocks, *centroids);
    } while ((lastError - currentError) / lastError > errorThreshold);

    // TODO deal with isolated centroids

    centroidsToCmpMutex.lock();
    centroidsToCompare.push({centroids, currentError});
    centroidsToCmpMutex.unlock();

    sem_post(centroidsToCmpAvailable);
}

void calculateClosestBlocks(
        int blocksBeginIdx,
        int blocksEndIdx,
        vector<vector<short>>& blocks,
        vector<vector<short>>* centroids,
        int numCentroids,
        vector<vector<vector<short>*>>& closestBlocksPerCentroid
        ) {
    double smallestLocalError, error;
    size_t localCentroidIdx;
    for (int i = blocksBeginIdx; i < blocksEndIdx; i++) {
        vector<short> block = blocks[i];

        smallestLocalError = calcEn(block, centroids->at(0));
        localCentroidIdx = 0;
        for (size_t j = 1; j < numCentroids; j++) {
            error = calcEn(block, centroids->at(j));
            if (error < smallestLocalError) {
                smallestLocalError = error;
                localCentroidIdx = j;
            }
        }

        closestBlocksPerCentroid[localCentroidIdx].push_back(&blocks[i]);
    }
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
                    int& numRuns, string& outputFile, int& numThreadsForRuns,
                    int& numThreadsForEachRun) {
    int argsOffset = argc == 9 ? 2 : 0;

    // validate input file
    checkFileToRead(sndFileIn, argv[argc - 6 - argsOffset]);
    int sndFileSize = sndFileIn.frames();

    // validate block size
    try {
        blockSize = stoi(argv[argc - 5 - argsOffset]);
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
        overlapFactor = stof(argv[argc - 4 - argsOffset]);
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
        errorThreshold = stof(argv[argc - 3 - argsOffset]);
    } catch (...) {
        cerr << "Error: errorThreshold must be a valid number" << endl;
        exit(1);
    }
    if (errorThreshold <= 0) {
        cerr << "Error: errorThreshold must be larger than zero" << endl;
        exit(1);
    }

    // validate number of runs
    try {
        numRuns = stoi(argv[argc - 2 - argsOffset]);
    } catch (...) {
        cerr << "Error: numRuns must be a valid number" << endl;
        exit(1);
    }
    if (numRuns <= 0) {
        cerr << "Error: numRuns must be larger than zero" << endl;
        exit(1);
    }

    outputFile = argv[argc - 1 - argsOffset];

    if (argc == 9) {
        try {
            numThreadsForRuns = stoi(argv[argc - 2]);
        } catch (...) {
            cerr << "Error: numThreadsForRuns must be a valid number" << endl;
            exit(1);
        }
        if (numThreadsForRuns <= 0) {
            cerr << "Error: numThreadsForRuns must be larger than zero" << endl;
            exit(1);
        }

        try {
            numThreadsForEachRun = stoi(argv[argc - 1]);
        } catch (...) {
            cerr << "Error: numThreadsForEachRun must be a valid number" << endl;
            exit(1);
        }
        if (numThreadsForEachRun <= 0) {
            cerr << "Error: numThreadsForEachRun must be larger than zero" << endl;
            exit(1);
        }
    }
    else {
        numThreadsForRuns = numThreadsForEachRun = 2;
    }
}
