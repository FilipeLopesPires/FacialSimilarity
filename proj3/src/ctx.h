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

#ifndef CTX_H
#define CTX_H

#include <opencv2/opencv.hpp>

class Ctx {
  private:
	std::vector<cv::Point> _coords;

  public:
	Ctx();
	Ctx(unsigned order, std::ifstream& ifs);
	Ctx(std::vector<cv::Point>&& points);
	unsigned size();
	void config(unsigned order, std::ifstream& ifs);
	std::string key(const cv::Mat& img, cv::Point current);
	std::vector<cv::Point>& coords();
	void dump();
	void dump(std::ofstream& ofs);
	void show();
};

#endif

