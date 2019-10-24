#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <stdlib.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {

	if(argc < 5) {
		cerr << "Usage: wavcb <input file> <block size> <overlap factor> <codebook size> <number of centroids> <output file>" << endl;
		return 1;
	}

    // validate input file
	SndfileHandle sndFileIn { argv[argc-6] };
	if(sndFileIn.error()) {
		cerr << "Error: invalid input file" << endl;
		return 1;
    }
	if((sndFileIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format" << endl;
		return 1;
	}
	if((sndFileIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format" << endl;
		return 1;
	}
    int sndFile_size = sndFileIn.frames();

    // validate block size
    int block_size;
    try {
        block_size = stoi(argv[argc-5]);
    } catch (...) {
        cerr << "Error: block size must be a valid number" << endl;
		return 1;
    }
    if(block_size <= 0) {
        cerr << "Error: block size must be larger than zero" << endl;
		return 1;
    }
    if(block_size > sndFile_size) {
        cerr << "Error: block size must be smaller than " << sndFile_size << endl;
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
    if(overlap_factor < 0 || overlap_factor > 1) {
        cerr << "Error: overlap factor must be a number between 0 and 1" << endl;
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
    if(codebook_size <= 0) {
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
    while((nFrames = sndFileIn.readf(block.data(), block_size))) {
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

    // classify blocks (apply k-means algorithm)
    // ...

    // randomly choose block (n times, n=number of centroids (is this passed as arguemnt))
    // vector de centroids
    // classificar cada block contra as centroids
        // calcular erro de cada block para cada centroid (ver formula no quadro)
        // encontrar centroid com menor erro para o block


	return 0;
}

