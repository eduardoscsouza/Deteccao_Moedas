#ifndef MASK_H
#define MASK_H


#include <opencv2/opencv.hpp>
#include <math.h>

typedef unsigned char uchar;

using namespace cv;

double euclDis(Mat&, int, int, int, int);
int sum(Mat&, int, int);
int valid(int, int, int, int);
void dfs(Mat&, Mat&, int, int, uchar **, double);
Mat generateMask(Mat&, double);
void applyMask(Mat&, Mat&);


#endif