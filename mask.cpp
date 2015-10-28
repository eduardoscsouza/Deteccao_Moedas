#include "mask.hpp"

#define NEIGH_TYPE 4
#define STD_MAX 255


double euclDis(Mat& im, int iA, int jA, int iB, int jB)
{
	double sumDif = 0;
	for (int k=0; k<im.channels(); k++)
		sumDif+= ((im.at<uchar>(iA, jA+k))-(im.at<uchar>(iB, jB+k))) *
			    ((im.at<uchar>(iA, jA+k))-(im.at<uchar>(iB, jB+k)));

	return sqrt(sumDif);
}


int sum(Mat& im, int i, int j)
{
	int sum=0;
	for (int k=0; k<im.channels(); k++)
		sum+=im.at<uchar>(i, j+k);

	return sum;
}


int valid(int i, int j, int height, int width)
{
	return
	(i>=0 && j>=0 && i<height && j<width);
}

void dfs(Mat& im, Mat& mask, int i, int j, uchar ** movement, double threshold)
{
	for (int k=0; k<im.channels(); k++) mask.at<uchar>(i, j+k)=0;

	int nextI, nextJ;
	for (int aux=0; aux<NEIGH_TYPE; aux++){
		nextI=i+movement[aux][0]; nextJ=j+(movement[aux][1]*im.channels());
		if (valid(nextI, nextJ, im.rows, im.cols*im.channels()) && euclDis(im, i, j, nextI, nextJ) < threshold
		&& sum(mask, nextI, nextJ)!=0)
			dfs(im, mask, nextI, nextJ, movement, threshold);
	}
}



Mat generateMask(Mat& im, double threshold)
{
	Mat filtered, mask;
	bilateralFilter(im, filtered, 12, 60, 60, BORDER_DEFAULT);
	mask = Mat(filtered.rows, filtered.cols, CV_8UC3);
	mask.setTo(cv::Scalar(STD_MAX, STD_MAX, STD_MAX));

	uchar ** movement = (uchar**) malloc(NEIGH_TYPE * sizeof(uchar*));
	for (int i=0; i<NEIGH_TYPE; i++) movement[i]=(uchar*) malloc(2*sizeof(uchar));

	movement[0][0]=0;movement[0][1]=1;
	movement[1][0]=1;movement[1][1]=0;
	movement[2][0]=0;movement[2][1]=-1;
	movement[3][0]=-1;movement[3][1]=0;

	dfs(filtered, mask, 0, 0, movement, threshold);

	for (int i=0; i<NEIGH_TYPE; i++) free(movement[i]);
	free(movement);

	return mask;
}


void applyMask(Mat& im, Mat& mask)
{
	for (int i=0; i<im.rows; i++)
		for (int j=0; j<im.cols*im.channels(); j++)
			if (mask.at<uchar>(i, j)==0)
				im.at<uchar>(i, j)=0;
}