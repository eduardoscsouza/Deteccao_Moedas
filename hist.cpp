#include "hist.hpp"

#define STD_POW 256
#define N_COMB 59
#define NEIGH_TYPE 8


Histogram * colourHist(Mat& im, Mat& mask, int regionValue)
{
	Histogram * hist = initHist(STD_POW * im.channels());

	ulong areaSize = 0;
	for (int i=0; i<im.rows; i++)
		for(int j=0; j<im.cols * im.channels(); j++)
			if (mask.at<uchar>(i, j)==regionValue){
				hist->elements[im.at<uchar>(i, j) + (j%im.channels())*STD_POW]++;
				areaSize++;
			}

	if (areaSize) for (ulong i=0; i<hist->total; i++) hist->elements[i]/=areaSize;

	return hist;
}



uchar * createLookup()
{
	uchar * lookup = (uchar*) malloc(STD_POW*sizeof(uchar));
	uchar index = 0;

	for (int i=0; i < STD_POW; i++) {
		int n = i ^ ((i >> 1) | (i << 7 & (STD_POW-1)));

		int count;
		for (count = 0; n; count++) n &= n-1;
		
		if (count <= 2) {
			lookup[i] = index;
			index++;
		}
		else lookup[i] = (N_COMB-1);
	}

	return lookup;
}


uchar calculateCel(Mat& im, int i, int j, char ** movement)
{
	uchar out=0;

	int compI, compJ;
	for (int aux=0; aux<NEIGH_TYPE; aux++){
		compI=i+movement[aux][0]; compJ=j+movement[aux][1];
		if (validPos(compI, compJ, im.rows, im.cols) &&
		im.at<uchar>(compI, compJ) >= im.at<uchar>(i, j))
			out = out | (1 << aux);
	}

	return out;
}


Histogram * textureHist(Mat& im, Mat& mask, int regionValue)
{
	Histogram * hist = initHist(N_COMB);

	Mat filtered;
	bilateralFilter(im, filtered, 5, 500, 500, BORDER_DEFAULT);

	Mat auxIm, auxMask;
	if (filtered.channels() != 1 || mask.channels() != 1){
		cvtColor(filtered, auxIm, CV_BGR2GRAY);
		cvtColor(mask, auxMask, CV_BGR2GRAY);
	}
	else{
		auxIm=filtered;
		auxMask=mask;
	}

	char ** movement = createMov(NEIGH_TYPE);
	uchar * lookup = createLookup();
	ulong areaSize = 0;
	for (int i=0; i<auxIm.rows; i++)
		for (int j=0; j<auxIm.cols; j++)
			if (auxMask.at<uchar>(i, j)==regionValue){
				hist->elements[lookup[calculateCel(auxIm, i, j, movement)]]++;
				areaSize++;
			}

	if (areaSize) for (ulong i=0; i<hist->total; i++) hist->elements[i]/=areaSize;

	freeMov(movement, NEIGH_TYPE);
	free(lookup);
	return hist;
}



void printHist(Histogram * hist, int imageHeigth, int imageWidth)
{
	Mat printImage = Mat::zeros(imageHeigth, imageWidth, CV_8UC3);

	double rectangleWidth = hist->total ? (imageWidth/(double)hist->total) : 0;
	double xPos=0;

	double maxCol=0;
	for (ulong i=0; i<3*STD_POW; i++) 
		if (hist->elements[i]>maxCol) maxCol=hist->elements[i];

	double maxtText=0;
	for (ulong i=3*STD_POW; i<hist->total; i++) 
		if (hist->elements[i]>maxtText) maxtText=hist->elements[i];

	for (ulong i=0; i<hist->total; i++){
		rectangle(
			printImage,
			Point(xPos, imageHeigth),
			Point(
				xPos+rectangleWidth,
				imageHeigth - (i < 3*STD_POW ?
					(imageHeigth*hist->elements[i]/maxCol) :
					(imageHeigth*hist->elements[i]/maxtText))),
			i < 3*STD_POW ?
				Scalar(
					i<STD_POW ? (STD_POW-1) : 0,
					i<2*STD_POW && i>=STD_POW ? (STD_POW-1) : 0,
					i>=2*STD_POW ? (STD_POW-1) : 0) :
				Scalar((STD_POW-1), (STD_POW-1), (STD_POW-1)),
			CV_FILLED, 8, 0
			);
			xPos+=rectangleWidth;
	}

	imshow("Histogram", printImage);
}


Histogram * initHist(ulong total)
{
	Histogram * hist = (Histogram*) malloc(sizeof(Histogram));
	hist->total = total;

	hist->elements = (double*) malloc(hist->total * sizeof(double));
	memset(hist->elements, 0, hist->total * sizeof(double));

	return hist;
}


void freeHist(Histogram * hist)
{
	free(hist->elements);
	free(hist);
}


Histogram * concHist(Histogram * histA, Histogram * histB)
{
	Histogram * histC = initHist(histA->total + histB->total);

	histC->elements = (double*) malloc(histC->total*sizeof(double));
	memcpy(histC->elements, histA->elements, histA->total*sizeof(double));
	memcpy(histC->elements+histA->total, histB->elements, histB->total*sizeof(double));

	return histC;
}