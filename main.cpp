#include <math.h>
#include <string.h>

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "mask.hpp"

#define STD_THRES 3

#define STD_BITS 8
#define STD_MAX 255

using namespace std;
using namespace cv;


typedef struct
{
	double * elements;
	ulong total;
}Histogram;


void printHist(Histogram hist, int imageHeigth, int imageWidth)
{
	Mat printImage = Mat::zeros(imageHeigth, imageWidth, CV_8UC3);

	int rectangleWidth = hist.total ? (imageWidth/hist.total) : 0;
	int xPos=0;

	double max=0;
	for (int i=0; i<hist.total; i++) 
		if (hist.elements[i]>max) max=hist.elements[i];
	cout<<max<<endl;

	for (int i=0; i<hist.total; i++){
		rectangle(printImage,
		Point(xPos, imageHeigth),
		Point(xPos+rectangleWidth, imageHeigth-(imageHeigth*hist.elements[i])),
		Scalar( i<255 ? 255 : 0, (255<i && i<(255*2)) ? 255 : 0, i>(255*2) ? 255 : 0), CV_FILLED, 8, 0);
		xPos+=rectangleWidth;
	}

	imshow("Histogram", printImage);
}



Histogram colourHist(Mat& im, Mat& mask, int regionValue)
{
	Histogram hist;
	hist.total = ((1<<STD_BITS) * im.channels());

	hist.elements = (double*) malloc(hist.total * sizeof(double));
	memset(hist.elements, 0, hist.total * sizeof(double));

	unsigned long areaSize = 0;
	for (int i=0; i<im.rows; i++) 
		for(int j=0; j<im.cols * im.channels(); j++){
			if (mask.at<uchar>(i, j/im.channels())==regionValue){
				hist.elements[im.at<uchar>(i, j)+((j%im.channels())*im.channels())]++;
				areaSize++;
			}
		}

	if (areaSize) for (int i=0; i<hist.total; i++) hist.elements[i]/=areaSize;

	return hist;
}


void useCamera(int resizeWidth)
{
	VideoCapture camera(0);
	Mat im, mask;
	while (1){
		camera>>im;
		resize(im, im, Size(resizeWidth*((double)im.cols/im.rows), resizeWidth));

		if(waitKey(30) >= 0) break;
	}
}



int main()
{
	int resizeWidth;
	string fileName;
	cin>>fileName>>resizeWidth;

	Mat im = imread(fileName, CV_LOAD_IMAGE_COLOR);
	resize(im, im, Size(resizeWidth*(((double)im.cols)/im.rows), resizeWidth));
	
	Mat mask = generateMask(im, STD_THRES);

	Histogram hist = colourHist(im, mask, STD_MAX);
	printHist(hist, 500, 900);

	waitKey(0);
	return 0;
}






