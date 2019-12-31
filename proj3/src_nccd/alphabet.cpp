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

#include "alphabet.h"

using namespace std;
using namespace cv;

Alphabet::Alphabet(const vector<Mat>& refs, const vector<Mat>& tars) {

	for(unsigned n = 0 ; n < refs.size() ; ++n)
		for(auto it = refs[n].begin<uchar>() ; it != refs[n].end<uchar>() ;++it)
			counts[(unsigned)*it]++;

	for(unsigned n = 0 ; n < tars.size() ; ++n)
		for(auto it = tars[n].begin<uchar>() ; it != tars[n].end<uchar>() ;++it)
			counts[(unsigned)*it]++;

	for(unsigned n = 0 ; n < 256 ; ++n)
		if(counts[n]) {
			cSymbols[n] = nSymbols;
			tSymbols[nSymbols++] = n;
		}
}

unsigned Alphabet::size() {
	return nSymbols;
}

Alphabet::CSymbol Alphabet::ts2cs(Alphabet::TSymbol ts) {
	return cSymbols[(unsigned)ts];
}

Alphabet::TSymbol Alphabet::cs2ts(Alphabet::CSymbol cs) {
	return tSymbols[cs];
}

