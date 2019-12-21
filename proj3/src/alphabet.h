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

#ifndef ALPHABET_H
#define ALPHABET_H

#include <opencv2/opencv.hpp>

class Alphabet {
  public:
	typedef uchar CSymbol;			// Coding symbol
	typedef uchar TSymbol;			// Text symbol

  private:
	CSymbol		cSymbols[256];		// TSymbol -> CSymbol
	TSymbol		tSymbols[256];		// CSymbol -> TSymbol
	unsigned	nSymbols = 0;		// Number of symbols
	unsigned	counts[256] = {};	// Counts of char usage in the file

  public:
    Alphabet(const std::vector<cv::Mat>& refs, const std::vector<cv::Mat>& tars);
	unsigned size();
	CSymbol ts2cs(TSymbol ts);
	TSymbol cs2ts(CSymbol cs);
};

#endif

