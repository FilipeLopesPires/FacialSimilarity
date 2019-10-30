#include <iostream>
#include <sndfile.hh>
#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "headers/snr.h" 
#include "headers/io.h"

using namespace std;

int main(int argc, char *argv[]) {

	if(argc < 5) {
		cerr << "Usage: wavcb <input file> <block size> <overlap factor> <codebook size> <number of centroids> <output file>" << endl;
		return 1;
	}

    // validate input file
    string filename = argv[argc-6];
	SndfileHandle sndFileIn { filename };
	checkFile(sndFileIn, filename, 1);      // change last argument in the future
    int sndFile_size = sndFileIn.frames();

    // validate block size
    int block_size;
    try {
        block_size = stoi(argv[argc-5]);
    } catch (...) {
        cerr << "Error: block size must be a valid number" << endl;
        return 1;
    }
    if (block_size <= 0) {
        cerr << "Error: block size must be larger than zero" << endl;
        return 1;
    }
    if (block_size > sndFile_size) {
        cerr << "Error: block size must be smaller than " << sndFile_size
             << endl;
        return 1;
    }

    // validate overlap factor
    float overlap_factor;
    try {
        overlap_factor = stof(argv[argc-4]);
    } catch (...) {
        cerr << "Error: overlap factor must be a valid number" << endl;
        return 1;
    }
    if (overlap_factor < 0 || overlap_factor > 1) {
        cerr << "Error: overlap factor must be a number between 0 and 1"
             << endl;
        return 1;
    }

    // validate codebook size
    int codebook_size;
    try {
        codebook_size = stoi(argv[argc-3]);
    } catch (...) {
        cerr << "Error: codebook size must be a valid number" << endl;
        return 1;
    }
    if (codebook_size <= 0) {
        cerr << "Error: codebook size must be larger than zero" << endl;
        return 1;
    }

    // validate number of centroids
    int num_centroids;
    try {
        num_centroids = stoi(argv[argc-2]);
    } catch (...) {
        cerr << "Error: number of centroids must be a valid number" << endl;
		return 1;
    }
    if(num_centroids <= 0) {
        cerr << "Error: number of centroids must be larger than zero" << endl;
		return 1;
    }

    // retrieve all blocks
    vector<vector<short>> blocks;
    vector<short> block(block_size * sndFileIn.channels());
    int nFrames;
    int i = 0;
    while ((nFrames = sndFileIn.readf(block.data(), block_size))) {
        blocks.push_back(block);
        //cout << blocks.at(i).at(0) << endl;
        sndFileIn.seek((block_size-(int)block_size*overlap_factor)*i, SEEK_SET);
        i++;
    }

    // validate number of centroids
    if(blocks.size() < num_centroids) {
        cerr << "Error: too many centroids for the given initial arguments (maximum = " + to_string(blocks.size()) + ")" << endl;
		return 1;
    }
    
    // initialize centroids
    int index, cur_centroidvec_size;
    vector<vector<short>> centroids;
    for(int i=0; i<num_centroids; i++) {
        cur_centroidvec_size = centroids.size();
        while(centroids.size() == cur_centroidvec_size) {
            index = rand() % blocks.size();
            if(!count(centroids.begin(),centroids.end(),blocks.at(index))) { // if centroids does not yet contain that possible centroid
                centroids.push_back(blocks.at(index));
            }
        }
    }
    /*cout << blocks.size() << endl;
    for(int i=0; i<num_centroids; i++) { cout << to_string(centroids.at(i).at(0)) << endl; }*/


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
        vector<short> new_local_centroid(block_size);
        vector<short> sum_blocks(block_size);
        total_num_blocks = 0;
        for(int j=0; j<closest_centroids.size(); j++) {
            if(centroids.at(i) == closest_centroids.at(j)) {
                for(int idx=0; idx<block_size; idx++){
                    sum_blocks[idx] += blocks[j][idx];
                    total_num_blocks += 1;
                }
            }
        }
        for(int idx=0; idx<block_size;idx++){
            new_local_centroid[idx]=(short)(sum_blocks[idx]/total_num_blocks);
        }
        new_centroids.push_back(new_local_centroid);
    }
    for(int i=0; i<centroids.size(); i++) {
        for(int f=0; f<block_size;f++){
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
