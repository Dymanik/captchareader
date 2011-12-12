#include <highgui.h>
#include <cv.h>
#include <ml.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    
    if(argc<3){
        cout << "Usage: ./" << argv[0] << " < Infile Net > < Test Case 1 >...< Test Case N >" << endl;
        exit(-1);
    }

    CvANN_MLP red;
    red.load(argv[1]);

    //Matrices
    Mat input, src;

    for(int i =2; i<argc; i++){

        //Se leen las imagenes por argumentos
        src = imread(argv[i],CV_LOAD_IMAGE_GRAYSCALE);

        //cout << argv[i][6] << argv[i][6]-'A' <<  endl;

        /*Mat out;
        out = Mat::zeros(1,26,CV_32FC1);
        out.at<float>(argv[i][6]-'A')=1;
        output.push_back(out);

        cout << out<<endl;*/

        if(!src.data){
            cout << "Error al leer la imagen" << endl;
            continue;
        }

        //Turns matrix to vector
        src = src.reshape(0,1);

        //Push it to the input matrix
        input.push_back(src);

        //cout << src<<endl;
    }

    //The number of training samples. 
    int train_sample_count = input.rows;

    //Get the number of samples.
    printf("Found training file with %d samples...\n", train_sample_count);

    //Create the matrices

    //Input data samples. Matrix of order (train_sample_count x 2)
    Mat trainData;
    input.convertTo(trainData,CV_32FC1);

    Mat closeenough;

    red.predict(trainData, closeenough);

    int k=0;
	int corr =0;
    for(int i=0; i<train_sample_count; i++){
		float max=-2;
		int m=0;
        for(int j=0; j<26; j++){
            if(closeenough.at<float>(i,j)>max){
                m=j;
				max=closeenough.at<float>(i,j);
            }
        }
        printf("%c",m+'A');
		if(k+'A'==m+'A')corr++;
        if((i+1)%100==0){
            printf(" %d\n",corr);
            k++;
			corr=0;
        }
    }
}
