#include <iostream>
#include <sndfile.hh>
#include <cstdlib>
#include <vector>
#include <set>

#include "headers/io.h"
#include "headers/vctQuant.h"

using namespace std;

void parseArguments(int argc, char* argv[],
                    SndfileHandle& sndFileIn, int& blockSize, float& overlapFactor, int &codebookSize, int& numCentroids);

int main(int argc, char *argv[]) {
	if(argc != 7) {
		cerr << "Usage: wavcb <input file> <block size> <overlap factor> <codebook size> <number of centroids> <output file>" << endl;
		return 1;
	}

    // parse and validate arguments
	SndfileHandle sndFileIn { argv[argc - 6] };
	int blockSize, codebookSize, numCentroids;
	float overlapFactor;
	parseArguments(argc, argv,
	               sndFileIn, blockSize, overlapFactor, codebookSize, numCentroids);

    // retrieve all blocks
    vector<vector<short>> blocks;
    retrieveBlocks(blocks, sndFileIn, blockSize, overlapFactor);

    // validate number of centroids
    if(blocks.size() < numCentroids) {
        cerr << "Error: too many centroids for the given initial arguments (maximum = " + to_string(blocks.size()) + ")" << endl;
		return 1;
    }
    
    // initialize centroids
    vector<vector<short>> centroids;
    {set<int> indexesUsed;
    int index, prev_centroidvec_size;
    for(int i=0; i<numCentroids; i++) {
        prev_centroidvec_size = centroids.size();
        while(centroids.size() == prev_centroidvec_size) {
            index = rand() % blocks.size();
            if (indexesUsed.count(index) == 0) {
                centroids.emplace_back(blocks.at(index));
                indexesUsed.insert(index);
            }
        }
    }}

    // generates centroids (apply k-means algorithm)
    {vector<vector<vector<short>*>> closest_blocks(centroids.size());
    {long error, smallest_local_error;
    int local_centroid_idx;
    for(auto& block : blocks) {
        smallest_local_error = calcEn(block, centroids.at(0));
        local_centroid_idx = 0;
        for(size_t j=1; j<centroids.size(); j++) {
            error = calcEn(block, centroids.at(j));
            if(error < smallest_local_error) {
                smallest_local_error = error;
                local_centroid_idx = j;
            }
        }
        closest_blocks
                .at(local_centroid_idx)
                .push_back(&block);
    } // for
    } // for's local variables

    // update centroids
    for(size_t i=0; i<centroids.size(); i++) {
        vector<long> sums_blocks(blockSize);
        for (auto& block : closest_blocks[i]) {
            for (size_t j = 0; j < block->size(); j++) {
                sums_blocks[j] += block->at(j);
            }
        }
        for(int idx=0; idx<blockSize;idx++){
            centroids[i][idx] = sums_blocks[idx] / closest_blocks[i].size();
            // TODO the code above causes an error (division by 0)
            //  in case no blocks where assigned (was closer to)
            //  to a certain centroid
        }
    } // for
    } // closest_blocks

    // randomly choose block (n times, n=number of centroids (is this passed as arguemnt))
    // vector de centroids
    // classificar cada block contra as centroids
        // calcular erro de cada block para cada centroid (ver formula no quadro)
        // encontrar centroid com menor erro para o block

    return 0;
}

void parseArguments(int argc, char* argv[],
                    SndfileHandle& sndFileIn, int& blockSize, float& overlapFactor, int &codebookSize, int& numCentroids) {

    // validate input file
    checkFileToRead(sndFileIn, argv[argc - 6]);
    int sndFileSize = sndFileIn.frames();

    // validate block size
    try {
        blockSize = stoi(argv[argc-5]);
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
        overlapFactor = stof(argv[argc-4]);
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
        codebookSize = stoi(argv[argc-3]);
    } catch (...) {
        cerr << "Error: codebook size must be a valid number" << endl;
        exit(1);
    }
    if (codebookSize <= 0) {
        cerr << "Error: codebook size must be larger than zero" << endl;
        exit(1);
    }

    // validate number of centroids
    try {
        numCentroids = stoi(argv[argc-2]);
    } catch (...) {
        cerr << "Error: number of centroids must be a valid number" << endl;
        exit(1);
    }
    if(numCentroids <= 0) {
        cerr << "Error: number of centroids must be larger than zero" << endl;
        exit(1);
    }

}
