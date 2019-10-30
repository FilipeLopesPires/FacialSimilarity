#include <iostream>
#include <sndfile.hh>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "headers/snr.h" 
#include "headers/io.h"

using namespace std;

void parseArguments(int argc, char* argv[],
                    SndfileHandle& sndFileIn, int& blockSize, float& overlapFactor, int &codebookSize, int& numCentroids);

int main(int argc, char *argv[]) {
	if(argc < 5) {
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
    vector<short> block(blockSize * sndFileIn.channels());
    int i = 0;
    while (sndFileIn.readf(block.data(), blockSize)) {
        blocks.push_back(block);
        //cout << blocks.at(i).at(0) << endl;
        sndFileIn.seek((blockSize-(int)blockSize*overlapFactor)*i, SEEK_SET);
        i++;
    }

    // validate number of centroids
    if(blocks.size() < numCentroids) {
        cerr << "Error: too many centroids for the given initial arguments (maximum = " + to_string(blocks.size()) + ")" << endl;
		return 1;
    }
    
    // initialize centroids
    int index, cur_centroidvec_size;
    vector<vector<short>> centroids;
    for(int i=0; i<numCentroids; i++) {
        cur_centroidvec_size = centroids.size();
        while(centroids.size() == cur_centroidvec_size) {
            index = rand() % blocks.size();
            if(!count(centroids.begin(),centroids.end(),blocks.at(index))) { // if centroids does not yet contain that possible centroid
                centroids.push_back(blocks.at(index));
            }
        }
    }
    /*cout << blocks.size() << endl;
    for(int i=0; i<numCentroids; i++) { cout << to_string(centroids.at(i).at(0)) << endl; }*/


    /*
        Código daqui para baixo não testado, penso que tem erros no formato dos blocks e dos centroids
    */

    // classify blocks (apply k-means algorithm)
    vector<vector<short>> closest_centroids;
    vector<long> smallest_errors;
    long error;
    long smallest_local_error;
    vector<short> local_centroid;
    for(int i=0; i<blocks.size(); i++) {
        smallest_local_error = calcEn(blocks.at(i), centroids.at(0));
        local_centroid = centroids.at(0);
        for(int j=1; j<centroids.size(); j++) {
            error = calcEn(blocks.at(i), centroids.at(j));
            if(error < smallest_local_error) {
                smallest_local_error = error;
                local_centroid = centroids.at(j);
            }
        }
        closest_centroids.push_back(local_centroid);
        smallest_errors.push_back(smallest_local_error);
    }
    vector<vector<short>> new_centroids;
    short total_num_blocks{};
    for(int i=0; i<centroids.size(); i++) {
        vector<short> new_local_centroid(blockSize);
        vector<short> sum_blocks(blockSize);
        total_num_blocks = 0;
        for(int j=0; j<closest_centroids.size(); j++) {
            if(centroids.at(i) == closest_centroids.at(j)) {
                for(int idx=0; idx<blockSize; idx++){
                    sum_blocks[idx] += blocks[j][idx];
                    total_num_blocks += 1;
                }
            }
        }
        for(int idx=0; idx<blockSize;idx++){
            new_local_centroid[idx]=(short)(sum_blocks[idx]/total_num_blocks);
        }
        new_centroids.push_back(new_local_centroid);
    }
    for(int i=0; i<centroids.size(); i++) {
        for(int f=0; f<blockSize;f++){
            cout << "Cur Centroid: " + to_string(centroids.at(i).at(f)) << endl;
            cout << "New Centroid: " + to_string(new_centroids.at(i).at(f)) << endl;
        }
    }
    






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
