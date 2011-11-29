
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <cv.h>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int main(int argc,char* argv[]){

	for(int i=1;i<argc;i++){
	Mat img = imread(argv[i],CV_LOAD_IMAGE_GRAYSCALE);
	Mat temp;
	if(!img.data ){ continue; }
		temp = img>128;
	Mat element = getStructuringElement( MORPH_RECT, Size( 3, 3 )); 
	/// Apply the dilation operation
	dilate( temp, img, element );
	erode( img, temp, element );

	resize(temp, img,Size(0,0) ,1.75 , 1.75, INTER_NEAREST);
	Rect roi(35,35,100,100);
//	imwrite("asd.png",img(roi));
	imwrite(argv[i],img(roi));


//	x=v*t+a*t/2
//	v=a*t
	}
	return 0;

}
