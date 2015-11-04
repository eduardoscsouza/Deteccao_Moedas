#ifndef HIST_H
#define HIST_H


#include <opencv2/opencv.hpp>
#include <string.h>
#include <stdlib.h>

#include "utils.hpp"

typedef unsigned char uchar;
typedef unsigned long int ulong;
typedef struct{
	ulong total;
	double * elements;
}Histogram;

using namespace cv;

Histogram * colourHist(Mat&, Mat&, int);

int validPos(int, int, int, int);
uchar * createLookup();
uchar calculateCel(Mat&, int, int, char**);
Histogram * textureHist(Mat&, Mat&, int);

Histogram * initHist(ulong);
void printHist(Histogram *, int, int);
void freeHist(Histogram *);
Histogram * concHist(Histogram *, Histogram *);

#endif


