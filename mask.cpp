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


void dfs(Mat& im, Mat& mask, int i, int j, char ** movement, double threshold)
{
	for (int k=0; k<im.channels(); k++) mask.at<uchar>(i, j+k)=0;

	int nextI, nextJ;
	for (int aux=0; aux<NEIGH_TYPE; aux++){
		nextI=i+movement[aux][0]; nextJ=j+(movement[aux][1]*im.channels());
		if (validPos(nextI, nextJ, im.rows, im.cols*im.channels()) && euclDis(im, i, j, nextI, nextJ) < threshold
		&& sum(mask, nextI, nextJ)!=0)
			dfs(im, mask, nextI, nextJ, movement, threshold);
	}
}


Mat generateMask(Mat& im, double threshold)
{
	Mat filtered, mask;
	bilateralFilter(im, filtered, 12, 120, 120, BORDER_DEFAULT);
	mask = Mat(filtered.rows, filtered.cols, CV_8UC3);
	mask.setTo(cv::Scalar(STD_MAX, STD_MAX, STD_MAX));

	char ** movement = createMov(NEIGH_TYPE);

	dfs(filtered, mask, 0, 0, movement, threshold);

	freeMov(movement, NEIGH_TYPE);
	return mask;
}



void applyMask(Mat& im, Mat& mask)
{
	for (int i=0; i<im.rows; i++)
		for (int j=0; j<im.cols*im.channels(); j++)
			if (mask.at<uchar>(i, j)==0)
				im.at<uchar>(i, j)=0;
}