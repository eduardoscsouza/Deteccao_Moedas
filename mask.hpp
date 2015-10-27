#include <opencv2/opencv.hpp>
#include <math.h>

#define NEIGH_TYPE 4
#define STD_MAX 255

using namespace cv;
using namespace std;

typedef unsigned char uchar;
typedef long unsigned int ulong;


double euclDis(Mat&, int, int, int, double);
int sum(Mat&, int, int);
int valid(int, int, int, int);
void dfs(Mat&, Mat&, int, int, uchar **, double);
Mat generateMask(Mat&, int);
void applyMask(Mat&, Mat&);