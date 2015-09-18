#include <stdio.h>

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define RESIZE_WIDTH 200
#define NEIGH_TYPE 4
#define DIF_THRES 15

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
	mask.at<unsigned char>(i, j)=regionValue;
	
	//imshow("grafo", mask);
	//waitKey(10);

	int aux, nextI, nextJ;
	for (aux=0; aux<NEIGH_TYPE; aux++){
		nextI=i+movement[aux][0]; nextJ=j+movement[aux][1];
		if (valid(nextI, nextJ, im.rows, im.cols*im.channels()) && sumDif(im, i, j, nextI, nextJ) < DIF_THRES
			&& mask.at<unsigned char>(nextI, nextJ)!=regionValue) dfs(im, mask, nextI, nextJ, movement, regionValue);
	}
}

Mat generateMask(Mat& im)
{
	Mat filtered, mask = Mat::zeros(im.rows, im.cols, CV_8UC3);
	bilateralFilter(im, filtered, 15, 80, 80, BORDER_DEFAULT);

	char movement[NEIGH_TYPE][2]
	{
		{ 1,  1},
		{ 1, -1},
		{-1,  1},
		{-1, -1}
	};

	dfs(filtered, mask, 0, 0, movement, 255);

	//imwrite("mascara.jpg", mask);
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
	resize(im, im, Size(RESIZE_WIDTH*((double)im.cols/im.rows), RESIZE_WIDTH));

	Mat mask = generateMask(im);

	return 0;
}