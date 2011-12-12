#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <cv.h>
#include <stdlib.h>
#include <stdio.h>
#include "captchaprep.h"

using namespace cv;

/* Noise reduction */

Mat lessnoise(char * imagen){
    Mat img = imread(imagen,CV_LOAD_IMAGE_GRAYSCALE);
	Mat temp;

	if(!img.data ){ continue; }
	temp = img>128;

	Mat element = getStructuringElement( MORPH_RECT, Size( 6, 6 )); 

	/// Apply the dilation operation
	dilate( temp, img, element );
	erode( img, temp, element );

    //Simple segmentation for 1 letter captcha
    Rect roi(20,20,60,60);
	resize(temp(roi), img,Size(40,40) ,0 , 0, INTER_NEAREST);

    return img;
	//imwrite(imagen,img);

}

/* Segmentation */


int main(int argc,char* argv[]){

	for(int i=1;i<argc;i++){

        Mat img;
        img=lessnoise(argv[i]);
    /*
		Mat img = imread(argv[i],CV_LOAD_IMAGE_GRAYSCALE);
		Mat temp;

		if(!img.data ){ continue; }
		temp = img>128;

		Mat element = getStructuringElement( MORPH_RECT, Size( 6, 6 )); 

		/// Apply the dilation operation
		dilate( temp, img, element );
		erode( img, temp, element );

    	Rect roi(20,20,60,60);
		resize(temp(roi), img,Size(40,40) ,0 , 0, INTER_NEAREST);*/

		imwrite(argv[i],img);
	}
	return 0;
}
