#ifndef KNN_H
#define KNN_H


#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hist.hpp"

typedef unsigned long int ulong;
typedef struct{
	int value;
	int votes; 
}Candidate;

using namespace cv;

size_t getFileSize(FILE *);
void ** getDatabase(int *);

int compareVotes(void *, void *, ulong);
int compareDist(void *, void *, ulong);
void mergeSort(void *, size_t, size_t, int (*)(void *, void *, ulong), ulong);

double getDist(double *, double *, ulong);
void calcDists(void **, int, Histogram *);

Candidate * election(void **, int, Histogram *, int);
int getClass(void **, int, Histogram *, int);


#endif