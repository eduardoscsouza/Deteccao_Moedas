#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char *argv[])
{
    IplImage *img, *cc_color, *cc_gray; /*IplImage is an image in OpenCV*/
    CvMemStorage *mem;
    CvSeq *contours, *ptr;
    img = cvLoadImage(argv[1], 0); /* loads the image from the command line */
    
    // cria imagens para receber os componentes conectados
    cc_color = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cc_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvZero(cc_gray);

    cvThreshold(img, img, 0, 255, CV_THRESH_OTSU);
    cvNamedWindow("Input",0);
    cvShowImage("Input", img);
    cvWaitKey(0); 
    
    mem = cvCreateMemStorage(0);
    cvFindContours(img, mem, &contours, sizeof(CvContour), CV_RETR_CCOMP,
        CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    
    int i = 1;
    for (ptr = contours; ptr != NULL; ptr = ptr->h_next) {
        CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);
        cvDrawContours(cc_color, ptr, color, CV_RGB(0,0,0), -1, CV_FILLED, 8, cvPoint(0,0));

        cvDrawContours(cc_gray, ptr, cvScalarAll(i), cvScalarAll(0), -1, CV_FILLED, 8, cvPoint(0,0));
	i++;
    }    
    
    cvNamedWindow("CC",0);
    cvShowImage("CC", cc_color);

    cvNamedWindow("CG",0);
    cvShowImage("CG", cc_gray);
    cvWaitKey(0); 

    cvSaveImage("result.png", cc_color);
    cvSaveImage("result_gray.png", cc_gray);
    cvReleaseImage(&img); 
    cvReleaseImage(&cc_color);
    cvReleaseImage(&cc_gray);
    return 0;
}