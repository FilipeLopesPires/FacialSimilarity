//------------------------------------------------------------------------------
//
// Copyright 2017 University of Aveiro, Portugal, All Rights Reserved.
//
// These programs are supplied free of charge for research purposes only,
// and may not be sold or incorporated into any commercial product. There is
// ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
// fit for ANY PURPOSE WHATSOEVER. Use them at your own risk. If you do
// happen to find a bug, or have modifications to suggest, please report
// the same to Armando J. Pinho, ap@ua.pt. The copyright notice above
// and this statement of conditions must remain an integral part of each
// and every copy made of these files.
//
// Armando J. Pinho (ap@ua.pt)
// IEETA / DETI / University of Aveiro
// 2017
//
//------------------------------------------------------------------------------

#ifndef FCM_H
#define FCM_H

#include <unordered_map>

template <class T> class FCM {
  private:
	class Counts {
	  private:
	  friend class FCM;
		T*			counts;
		unsigned	sum;

	  public:
		Counts(unsigned a) { counts = new T[a] { } ; sum = 0; }
		// "Counts" objects cannot be copied (only moved)
		Counts(const Counts&) {
			std::cerr << "Illegal call to copy constructor of Counts" << std::endl;
			exit(0);
		}
		Counts(Counts&& cnts) {
			sum = cnts.sum; counts = cnts.counts; cnts.counts = nullptr;
		}
		~Counts() { if(counts) delete[] counts; }
	};

	typedef struct {
		T			count;
		unsigned	sum;
	} Count;

	unsigned	nSymbols;
	unsigned	maxCount;
	unsigned	alphaN = 1; // Numerator of alpha
	unsigned	alphaD = 1; // Denominator of alpha
	double		alpha = 1;
	typename std::unordered_map<std::string, Counts>::iterator counters_it;
	std::unordered_map<std::string, Counts> counters;

  public:
	double *probs;

  public:
	FCM(unsigned a);
	void inc(const std::string&& key, unsigned symbol);
	void select(const std::string&& key);
	Count count(unsigned symbol);
	double bits_needed(unsigned symbol, double alpha = 1);
	void dump();
	void dump_totals();
	void set_alpha(unsigned n, unsigned d);
	void compute_probs();
};

#endif

