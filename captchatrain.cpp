#include <highgui.h>
#include <cv.h>
#include <ml.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char * argv[]){

    if(argc<3){
        cout << "Usage: ./" << argv[0] << " < Outfile Net > < Train Case 1 >...< Train Case N >" << endl;
        exit(-1);
    }

    //Red neuronal
    CvANN_MLP red;

    //Matrices
    Mat input, output, src, dst;

    //Shuffling data
    int arr[argc-2];
    for(int i=2;i<argc;i++){
        arr[i]=i;
    }

    srand(time (NULL));
    int swap1, swap2, temp;

    for(int i=0; i<10000; i++){
        swap1=rand()%argc-2;
        swap2=rand()%argc-2;

        if(swap1==swap2){
            i--;
        }else{
            temp= array[swap1];
            array[swap1]=array[swap2];
            array[swap2]=temp;
        }
    }

    for(int i =2; i<argc; i++){

        //Se leen las imagenes por argumentos
        src = imread(argv[arr[i]],CV_LOAD_IMAGE_GRAYSCALE);

        //cout << argv[i][6] << argv[i][6]-'A' <<  endl;

        Mat out(1,26,CV_32FC1,Scalar(-1));
        out.at<float>(argv[i][6]-'A')=1;
        output.push_back(out);

        //cout << out<<endl;

        if(!src.data){
            cout << "Error al leer la imagen" << endl;
            continue;
        }

        //cout<<src<<endl;

        src = src.reshape(0,1);

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

    //Output data samples. Matrix of order (train_sample_count x 1)
    Mat trainClasses = Mat(output);

    //The weight of each training data sample. We'll later set all to equal
    //weights.
    Mat sampleWts =  Mat::ones(train_sample_count, 1, CV_32FC1);

    //The matrix representation of our ANN. We'll have 3 layers.
    Mat neuralLayers(3, 1, CV_32SC1);

    //Setting the number of neurons on each layer of the ANN

    /*We have in Layer 1: 1600 neurons (2 inputs)
      Layer 2: 150 neurons (hidden layer)
      Layer 3: 26 neurons (1 output)
     */
    neuralLayers.at<int>(0)=1600;
    neuralLayers.at<int>(1)=150;
    neuralLayers.at<int>(2)=26;

    //Create our ANN.
    red.create(neuralLayers);

    //Train it with our data.
    red.train(
            trainData,
            trainClasses,
            sampleWts,
            Mat(),
            CvANN_MLP_TrainParams(
                cvTermCriteria(
                    CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                    100000,
                    1.0
                    ),
                CvANN_MLP_TrainParams::BACKPROP,
                0.01,
                0.05
                )
            );

    red.save(argv[1]);
}
