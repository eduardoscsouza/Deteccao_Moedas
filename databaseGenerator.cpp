#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>

#include "mask.hpp"
#include "hist.hpp"

#define STD_THRES 1.5
#define STD_MAX 255

using namespace std;
using namespace cv;


int main()
{
	char auxString[256];
	int resizeWidth, classe, show;
	scanf("%s%d%d", auxString, &resizeWidth, &show);

	FILE * input = fopen(auxString, "r");
	if (input==NULL) {
		printf("Unnable to open file!\n");
		return -1;
	}

	FILE * output = fopen("database.dat", "w");
	if (output==NULL) {
		printf("Unnable to open file!\n");
		return -1;
	}

	Mat im, mask;
	Histogram *colHist, *texHist, *hist; 
	while (fscanf(input, "%s%d", auxString, &classe)!=EOF){
		printf("%s\n", auxString);
		im = imread(auxString, CV_LOAD_IMAGE_COLOR);
		resize(im, im, Size(resizeWidth*(((double)im.cols)/im.rows), resizeWidth));

		mask = generateMask(im, STD_THRES);
		
		colHist = colourHist(im, mask, STD_MAX);
		texHist = textureHist(im, mask, STD_MAX);
		hist = concHist(colHist, texHist);
		freeHist(colHist);
		freeHist(texHist);

		fwrite(&(hist->total), sizeof(ulong), 1, output);
		fwrite(hist->elements, sizeof(double)*hist->total, 1, output);
		fwrite(&classe, sizeof(int), 1, output);

		if (show){
			applyMask(im, mask);
			imshow(auxString, im);
			waitKey(0);
			destroyWindow(auxString);
		}
		im.release();
		mask.release();

		freeHist(hist);
	}

	fclose(input);
	fclose(output);

	return 0;
}






