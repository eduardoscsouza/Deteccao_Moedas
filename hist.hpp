#ifndef HIST_H
#define HIST_H


#include <opencv2/opencv.hpp>

typedef unsigned char uchar;
typedef unsigned long int ulong;
typedef struct{
	double * elements;
	ulong total;
}Histogram;


using namespace cv;

Histogram * colourHist(Mat&, Mat&, int);
void printHist(Histogram *, int, int);


#endif


