#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "mask.hpp"
#include "hist.hpp"

#define STD_THRES 7.5
#define STD_MAX 255

using namespace std;
using namespace cv;


void useCamera(int resizeWidth)
{
	VideoCapture camera(0);
	Mat im, mask;
	while (1){
		camera>>im;
		resize(im, im, Size(resizeWidth*((double)im.cols/im.rows), resizeWidth));

		//codigo

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

	Histogram * hist = colourHist(im, mask, STD_MAX);
	printHist(hist, 500, 900);

	waitKey(0);
	free(hist);
	return 0;
}






