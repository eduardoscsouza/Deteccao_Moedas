#include "hist.hpp"

#define STD_MAX 255


Histogram * colourHist(Mat& im, Mat& mask, int regionValue)
{
	Histogram * hist = (Histogram*) malloc(sizeof(Histogram));
	hist->total = (STD_MAX+1) * im.channels();

	hist->elements = (double*) malloc(hist->total * sizeof(double));
	memset(hist->elements, 0, hist->total * sizeof(double));

	ulong areaSize = 0;
	for (int i=0; i<im.rows; i++)
		for(int j=0; j<im.cols * im.channels(); j++)
			if (mask.at<uchar>(i, j)==regionValue){
				hist->elements[im.at<uchar>(i, j) + (j%im.channels())*(STD_MAX+1)]++;
				areaSize++;
			}

	if (areaSize) for (ulong i=0; i<hist->total; i++) hist->elements[i]/=areaSize;

	return hist;
}


void printHist(Histogram * hist, int imageHeigth, int imageWidth)
{
	Mat printImage = Mat::zeros(imageHeigth, imageWidth, CV_8UC3);

	double rectangleWidth = hist->total ? (imageWidth/(double)hist->total) : 0;
	double xPos=0;

	double max=0;
	for (ulong i=0; i<hist->total; i++) 
		if (hist->elements[i]>max) max=hist->elements[i];

	for (ulong i=0; i<hist->total; i++){
		rectangle(
			printImage,
			Point(xPos, imageHeigth),
			Point(xPos+rectangleWidth, imageHeigth-(imageHeigth*hist->elements[i]/max)),
			Scalar(
				i<=STD_MAX ? STD_MAX : 0,
				i<=2*STD_MAX && i>STD_MAX ? STD_MAX : 0,
				i>2*STD_MAX ? STD_MAX : 0
				),
			CV_FILLED, 8, 0
			);
			xPos+=rectangleWidth;
	}

	imshow("Histogram", printImage);
}