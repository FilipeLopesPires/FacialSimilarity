#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel]) 
			std::cout << value << '\t' << counter << '\n';
	}

	void fwrite(const size_t channel, const char* fname) const {
		std::ofstream file;
		file.open(fname);
		for(auto [value, counter] : counts[channel]) 
			file << value << '\t' << counter << '\n';
		file.close();
	}
};

#endif