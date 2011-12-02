#include <highgui.h>
#include <cv.h>
#include <ml.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    
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

    cout << closeenough << endl;
}
