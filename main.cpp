#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "mask.hpp"
#include "hist.hpp"
#include "knn.hpp"

#define STD_THRES 1.5
#define STD_MAX 255
#define STD_KNN 5

using namespace std;
using namespace cv;


void useCamera(int resizeWidth)
{
	VideoCapture camera(0);
	Mat im, mask;
	Histogram *colHist, *texHist, *hist;
	int dataCount;
	void ** database = getDatabase(&dataCount);
	while (1){
		camera>>im;
		resize(im, im, Size(resizeWidth*((double)im.cols/im.rows), resizeWidth));

		mask = generateMask(im, STD_THRES);
		applyMask(im, mask);
		
		colHist = colourHist(im, mask, STD_MAX);
		texHist = textureHist(im, mask, STD_MAX);
		hist = concHist(colHist, texHist);
		freeHist(colHist);
		freeHist(texHist);

		getClass(database, dataCount, hist, STD_KNN);

		calcDists(database, dataCount, hist);
		for (int i=STD_KNN*2; i>=0; i--)
			printf("%lf %05d\n",
			*((double*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total)+sizeof(int)))),
			(*((int*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total))))));
		printf("\n\n\n\n");
		
		freeHist(hist);

		imshow("currentMasked", im);
		if(waitKey(30) >= 0) break;
	}
}


int main()
{
	int resizeWidth;
	string fileName;
	cin>>fileName>>resizeWidth;
	//useCamera(resizeWidth);

	Mat im = imread(fileName, CV_LOAD_IMAGE_COLOR);
	resize(im, im, Size(resizeWidth*(((double)im.cols)/im.rows), resizeWidth));
	
	Mat mask = generateMask(im, STD_THRES);

	Histogram * colHist = colourHist(im, mask, STD_MAX);
	Histogram * texHist = textureHist(im, mask, STD_MAX);
	Histogram * hist = concHist(colHist, texHist);
	freeHist(colHist);
	freeHist(texHist);

	int dataCount;
	void ** database = getDatabase(&dataCount);
	getClass(database, dataCount, hist, STD_KNN);

	//ver a vizinhanca
	for (int i=STD_KNN*2; i>=0; i--)
		printf("%lf %05d\n",
		*((double*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total)+sizeof(int)))),
		(*((int*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total))))));
	
	/*
	calcDists(database, dataCount, hist);
	for (int i=0; i<dataCount; i++){
		printf("%d\n", *((int*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total)))));
	}
	*/

	freeHist(hist);

	applyMask(im, mask);
	imshow(fileName, im);
	waitKey(0);
	return 0;
}






