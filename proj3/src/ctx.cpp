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

#include <fstream>
#include "ctx.h"

using namespace std;
using namespace cv;

Ctx::Ctx() {
}

Ctx::Ctx(unsigned order, ifstream& ifs) {
	_coords = vector<Point> { order };
	for(unsigned n = 0 ; n < order ; ++n) {
		ifs >> _coords[n].y;
		ifs >> _coords[n].x;
	}
}

Ctx::Ctx(vector<Point>&& pnts) {
	_coords = pnts;
}

void Ctx::config(unsigned order, ifstream& ifs) {
	_coords = vector<Point> { order };
	for(unsigned n = 0 ; n < order ; ++n) {
		ifs >> _coords[n].y;
		ifs >> _coords[n].x;
	}
}

string Ctx::key(const Mat& img, Point current) {
	string key(_coords.size(), 0);

	for(unsigned n = 0 ; n < _coords.size() ; ++n) {
		Point p { current + _coords[n] };
		if(p.x < img.cols && p.x >= 0 && p.y < img.rows && p.y >= 0)
			key[n] = img.at<uchar>(p);
	}

	return key;
}

unsigned Ctx::size() {
	return _coords.size();
}

vector<Point>& Ctx::coords() {
	return _coords;
}

void Ctx::dump() {
	cout << _coords.size() << " ";
	for(unsigned n = 0 ; n < _coords.size() ; ++n) {
		cout << _coords[n].y << " " << _coords[n].x << " ";
	}

	cout << endl;
}

void Ctx::dump(ofstream& ofs) {
	ofs << _coords.size() << " ";
	for(unsigned n = 0 ; n < _coords.size() ; ++n) {
		ofs << _coords[n].y << " " << _coords[n].x << " ";
	}

	ofs << endl;
}

void Ctx::show() {
	int rMin, rMax; rMin = rMax = _coords[0].y;
	int cMin, cMax; cMin = cMax = _coords[0].x;

	for(unsigned n = 1 ; n < _coords.size() ; n++) {
		if(_coords[n].y > rMax)
			rMax = _coords[n].y;

		if(_coords[n].y < rMin)
			rMin = _coords[n].y;

		if(_coords[n].x > cMax)
			cMax = _coords[n].x;

		if(_coords[n].x < cMin)
			cMin = _coords[n].x;

	}

	Mat configMat { rMax-rMin+1, cMax-cMin+1, CV_32S, Scalar {0} };

	for(unsigned n = 0 ; n < _coords.size() ; n++)
		configMat.at<int>(_coords[n].y - rMin, _coords[n].x - cMin) = n + 1;

	configMat.at<int>(-rMin,-cMin) = -1;

	MatIterator_<int> it = configMat.begin<int>();
	for(it = configMat.begin<int>() ; it != configMat.end<int>() ; ++it) {
		switch(*it) {
			case -1:
				cout << "  X";
				break;

			case 0:
				cout << "   ";
				break;

			default:
				printf("%3d", *it);
		}

		if(it.pos().x == configMat.cols-1)
			cout << endl;

	}

}

