#include <stdio.h>

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define RESIZE_WIDTH 60.0
#define NEIGH_TYPE 4
#define DIF_THRES 50

using namespace std;
using namespace cv; 


int sumDif(Mat& im, int iA, int jA, int iB, int jB)
{
	int sum, k;
	for (sum=0, k=0; k<im.channels(); k++) sum+= im.at<unsigned char>(iA, jA+k) - im.at<unsigned char>(iB, jB+k);
	sum = sum >= 0 ? sum : -sum;

	return sum; 
}

int valid(int i, int j, int height, int width)
{
	return (i>=0 && j>=0 && i<height && j<width);
}

void dfs(Mat& im, Mat& mask, int i, int j, char movement[NEIGH_TYPE][2], int regionValue)
{
	if (mask.at<unsigned char>(i, j)==regionValue) return;
	mask.at<unsigned char>(i, j)=regionValue;

	imshow("grafo", mask);
	waitKey(10);

	int aux, nextI, nextJ;
	for (aux=0; aux<NEIGH_TYPE; aux++){
		nextI=i+movement[aux][0]; nextJ=j+movement[aux][1];
		if (valid(nextI, nextJ, im.rows, im.cols) && sumDif(im, i, j, nextI, nextJ) < DIF_THRES
			&& mask.at<unsigned char>(nextI, nextJ)==0) dfs(im, mask, nextI, nextJ, movement, regionValue);
	}
}

int getStartingPos(Mat& mask, int * i, int * j)
{
	for (NULL; *i<mask.rows; (*i)++){
		for(NULL; *j<mask.cols; (*j)++) if (mask.at<unsigned char>(*i, *j)==0) return 1;
		*j=0;
	}
	return 0;
}

Mat generateMask(Mat& im, int * nRegions)
{
	Mat filtered, mask = Mat::zeros(im.rows, im.cols, CV_8UC1);
	bilateralFilter(im, filtered, 15, 80, 80, BORDER_DEFAULT);

	char movement[NEIGH_TYPE][2]
	{
		{ 1,  1},
		{ 1, -1},
		{-1,  1},
		{-1, -1}
	};

	*nRegions=50;
	int startingI=0, startingJ=0;
	while(getStartingPos(mask, &startingI, &startingJ)) {
		dfs(filtered, mask, startingI, startingJ, movement, *nRegions);
		(*nRegions)+=50;
	}

	return mask;
}


int main()
{
	string fileName;
	cin>>fileName;

	Mat im = imread(fileName, CV_LOAD_IMAGE_COLOR);
	if (im.data==NULL){
		cout<<"Unnable to open file!"<<endl;
		return -1;
	}
	resize(im, im, Size((RESIZE_WIDTH/im.rows) * im.cols, RESIZE_WIDTH));


	int nRegions;
	Mat mask = generateMask(im, &nRegions);

	return 0;
}