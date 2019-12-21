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

#include <iostream>
#include <cmath>
#include "fcm.h"

using namespace std;

template class FCM<unsigned char>;
template class FCM<unsigned short>;
template class FCM<unsigned int>;

template <class T> FCM<T>::FCM(unsigned a) : nSymbols { a } {
	maxCount = (1ULL << sizeof(T) * 8) - 1;
	// (small) mem leak not solved, but faster than vector...
	probs = new double[nSymbols];
}

template <class T> void FCM<T>::select(const string&& key) {
	counters_it = counters.find(key);
}

template <class T> void FCM<T>::inc(const string&& key, unsigned symbol) {
	counters_it = counters.find(key);
	if(counters_it == counters.end()) {
		counters_it = counters.insert(make_pair(key, Counts { nSymbols })).first;
	}

	// if needed, divide by two to avoid overflow
	if(++counters_it->second.counts[symbol] == maxCount) {
		counters_it->second.sum = 0;
		for(unsigned n = 0 ; n < nSymbols ; ++n) {
			counters_it->second.sum += counters_it->second.counts[n] >>= 1;
		}
	}

	else {
		counters_it->second.sum++;
	}
}

template <class T> void FCM<T>::set_alpha(unsigned n, unsigned d) {
	alphaN = n; alphaD = d; alpha = (double)n/d;
}

template <class T> double FCM<T>::bits_needed(unsigned symbol, double alpha) {
	Count c = count(symbol);
	return -log2((c.count + alpha) / (c.sum + alpha * nSymbols));
}

template <class T> typename FCM<T>::Count FCM<T>::count(unsigned symbol) {
	Count c {};
	if(counters_it != counters.end()) {
		c.count = counters_it->second.counts[symbol];
		c.sum = counters_it->second.sum;
	}

	return c;
}

template <class T> void FCM<T>::compute_probs() {
	if(counters_it != counters.end()) {
		for(unsigned s = 0 ; s < nSymbols ; ++s) {
			probs[s] = (counters_it->second.counts[s] + alpha) /
			  (counters_it->second.sum + alpha * nSymbols);
		}
	}

	else {
		for(unsigned s = 0 ; s < nSymbols ; ++s) {
			probs[s] = 1.0 / nSymbols;
		}

	}

}

template <class T> void FCM<T>::dump_totals() {
	for(auto& c: counters) {
		cout << c.first << " : " << (unsigned)c.second.sum << endl;
	}
}

template <class T> void FCM<T>::dump() {
	for(auto& c: counters) {
		cout << c.first << " : ";
		for(unsigned n = 0 ; n < nSymbols ; ++n)
			cout << (unsigned)c.second.counts[n] << " ";

		cout << ": " << (unsigned)c.second.sum << endl;
	}
}

