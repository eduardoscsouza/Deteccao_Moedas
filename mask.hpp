#ifndef MASK_H
#define MASK_H


#include <opencv2/opencv.hpp>
#include <math.h>

#include "utils.hpp"

typedef unsigned char uchar;

using namespace cv;

double euclDis(Mat&, int, int, int, int);
int sum(Mat&, int, int);
void dfs(Mat&, Mat&, int, int, char **, double);
Mat generateMask(Mat&, double);

void applyMask(Mat&, Mat&);


#endif