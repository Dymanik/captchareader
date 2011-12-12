#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include <ml.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

 void lessnoise(Mat imag,Mat &ret){
	Mat temp;
	Mat img;
	temp = imag>128;

	Mat element = getStructuringElement( MORPH_RECT, Size( 5, 5 )); 

	/// Apply the dilation operation
	dilate( temp, img, element );
	erode( img, ret, element );

}
//red33.xml red35.xml red356dos.xml
int main(int argc,char *argv[]){
	CvANN_MLP red1,red2,red3;
	red1.load("red33.xml");
	red2.load("red35.xml");
	red3.load("red356dos.xml");

	for(int n =1; n<argc; n++){
		Mat img,imag;
		imag = imread(argv[n],CV_LOAD_IMAGE_GRAYSCALE);
		lessnoise(imag,img);

		int xs=0;
		int xf=0;
		int part=1;
		bool border=false;
		Mat input;

		imwrite("p.png",img);

		for(int i=0;i<img.cols;i++){
			int sum =0;
			for (int j=0;j<img.rows;j++){
				//			cout<<(img.at<char>(j,i)==0);
				if(img.at<char>(j,i)==0)sum++;
			}
			//		cout<<border<<' '<<i<<' '<<sum<<endl;
			if(sum>5){
				if(!border){
					border=true;
					xs=i;
				}
			}else{
				if(border){
					xf=i;
					int parts =1;
					int mid =0;
					if(xf-xs>60){
						parts =2;
						mid = (xf-xs)/2;
					}
					if(parts==1){
						Rect roi(xs,20,xf-xs,60);
												char F[10];
												sprintf(F,"p%d.png",part);
												imwrite(F,img(roi));
						 						Mat letter(60,60,CV_8UC1,255);
						Rect ROI((60-xf+xs)/2,0,xf-xs,60);
						Mat let = letter(ROI);

						int su =0;
						for(int k=xs;k<xf;k++){
							for (int m=20;m<80;m++){
						//		cout<<(img.at<char>(m,k)==0);
								if(img.at<char>(m,k)==0)su++;
							}
						//	cout<<endl;
						}
//						cout<<su<<' ';
						if(su<100){
							border=false;
							continue;
						}

						img(roi).copyTo(let);
						resize(letter, letter,Size(40,40) ,0 , 0, INTER_NEAREST);
						sprintf(F,"q%d.png",part);
						imwrite(F,letter);
						letter=letter.reshape(0,1);
						input.push_back(letter);
						part++;
					}else{
						{
							Rect roi(xs,20,mid,60);
							/*							char F[10];
														sprintf(F,"p%d.png",part);
														imwrite(F,img(roi));
							 */							Mat letter(60,60,CV_8UC1,255);
							Rect ROI((60-mid)/2,0,mid,60);
							Mat let = letter(ROI);
							img(roi).copyTo(let);
							resize(letter, letter,Size(40,40) ,0 , 0, INTER_NEAREST);
							//							sprintf(F,"q%d.png",part);
							//							imwrite(F,letter);
							letter=letter.reshape(0,1);
							input.push_back(letter);
							part++;
						}
						{Rect roi(xs+mid,20,mid,60);
							/*							char F[10];
														sprintf(F,"p%d.png",part);
														imwrite(F,img(roi));
							 */							Mat letter(60,60,CV_8UC1,255);
							Rect ROI((60-mid)/2,0,mid,60);
							Mat let = letter(ROI);
							img(roi).copyTo(let);
							resize(letter, letter,Size(40,40) ,0 , 0, INTER_NEAREST);
							//							sprintf(F,"q%d.png",part);
							//							imwrite(F,letter);
							letter=letter.reshape(0,1);
							input.push_back(letter);
							part++;
						}
					}

				}
				border=false;
			}
		}

		Mat trainData;
		input.convertTo(trainData,CV_32FC1);

		Mat closeenough1;
		Mat closeenough2;
		Mat closeenough3;

		red1.predict(trainData, closeenough1);
		red2.predict(trainData, closeenough2);
		red3.predict(trainData, closeenough3);

		float sum[26];
		int k=0;
		int corr =0;
		char res[15];
		memset(res,0,sizeof(char)*15);
		int i=0;
		for(;i<trainData.rows;i++){
			memset(sum,0,sizeof(float)*26);
			float max=-2;
			int m=0;
			for(int j=0; j<26; j++){
				sum[j]=closeenough1.at<float>(i,j)+closeenough2.at<float>(i,j)+closeenough3.at<float>(i,j); 
				if(sum[j]>max){
					m=j;
					max=sum[j];
				}
			}
			printf("%c",m+'A');
			res[i]=m+'A';
		}

		char savepath[30];
		sprintf(savepath,"results/%s.png",res);
		imwrite(savepath,imag);
		printf("\n");

	}
	return 0;
}
