#include <iostream>
#include <cstdint>
#include "ConvolutionNeuralNetwork.h"

using namespace ALib;

void LoadDataBase(const char *labelName,const char *imagesName,Matrix &input,Matrix &expected){

    FILE *f = fopen(labelName,"rb");

    unsigned labelCount,temp;
    fread(&labelCount,4,1,f);
    fread(&labelCount,4,1,f);
    {
        union test {
            unsigned char c[4];
            unsigned number;
        } testUnion;
        testUnion.number = labelCount;
        unsigned char help[4];
        help[0]=testUnion.c[3];
        help[1]=testUnion.c[2];
        help[2]=testUnion.c[1];
        help[3]=testUnion.c[0];

        testUnion.c[0]=help[0];
        testUnion.c[1]=help[1];
        testUnion.c[2]=help[2];
        testUnion.c[3]=help[3];

        labelCount=testUnion.number;
    }

    labelCount=10000;

    Matrix newExpected(labelCount,10);
    newExpected.SetZero();

    uint8_t smol;


    for(unsigned i=0;i<labelCount;i++){
        fread(&smol,1,1,f);
        newExpected[smol][i]=1;
    }

    fclose(f);
    expected=newExpected;
    f= fopen(imagesName,"rb");

    fread(&temp,4,1,f);
    fread(&temp,4,1,f);

    unsigned imageSizeX,imageSizeY;

    fread(&imageSizeY,4,1,f);
    fread(&imageSizeX,4,1,f);
    imageSizeY=28;
    imageSizeX=28;

    uint8_t tempSmal;
    float tempFloat;
    unsigned imageVectorSize=imageSizeX*imageSizeY;

    Matrix newInput(labelCount,imageVectorSize);


    for(unsigned i=0;i<labelCount;i++){
        for(int y=0;y<imageVectorSize;y++){
            fread(&tempSmal,1,1,f);
            tempFloat=tempSmal;
            tempFloat/=255.0f;
            newInput[y][i]=tempFloat;
        }
    }

    fclose(f);
    input=newInput;
}


void Zad1(){
    Matrix inputImage,filter,output;
    inputImage={
            {1,1,1,0,0},
            {0,1,1,1,0},
            {0,0,1,1,1},
            {0,0,1,1,0},
            {0,1,1,0,0}
    };
    filter={
            {1,0,1},
            {0,1,0},
            {1,0,1}
    };
    int stride=1;

    output= ConvolutionTask1(inputImage,filter,stride);
    std::cout<<"Zad 1"<<std::endl;
    std::cout<<output<<std::endl;
}


void Zad2(){

    std::cout<<"Zad 2"<<std::endl;

    Matrix filters(9,16);
    filters.Randomize(-0.01,0.01);
    Matrix outputLater(10816,10); //10816 to 16 filtrów x 676 powarstwowanych obrazów
    outputLater.Randomize(-0.1,0.1);

    Matrix testData,testExpected;
    Matrix trainData,trainExpected;
    LoadDataBase("../t10k-labels.idx1-ubyte","../t10k-images.idx3-ubyte",testData,testExpected);
    LoadDataBase("../train-labels.idx1-ubyte","../train-images.idx3-ubyte",trainData,trainExpected);




    for(int i=0;i<50;i++){
        int score=Test2(testData,testExpected,filters,outputLater);
        std::cout<<"Era "<<i+1<<" "<<score<<"/10000 "<<(float)score/100.0f<<"%"<<std::endl;
        ConvolutionUpdate2(trainData,trainExpected,filters,outputLater,0.01);
    }






}


void Zad3(){

    std::cout<<"Zad 3"<<std::endl;

    Matrix filters(9,16);
    filters.Randomize(-0.01,0.01);
    Matrix outputLater(2704,10); //10816 to 16 filtrów x 676 powarstwowanych obrazów/4pooling
    outputLater.Randomize(-0.1,0.1);

    Matrix testData,testExpected;
    Matrix trainData,trainExpected;
    LoadDataBase("../t10k-labels.idx1-ubyte","../t10k-images.idx3-ubyte",testData,testExpected);
    LoadDataBase("../train-labels.idx1-ubyte","../train-images.idx3-ubyte",trainData,trainExpected);




    for(int i=0;i<50;i++){
        int score=Test3(testData,testExpected,filters,outputLater);
        std::cout<<"Era "<<i+1<<" "<<score<<"/10000 "<<(float)score/100.0f<<"%"<<std::endl;
        ConvolutionUpdate3(trainData,trainExpected,filters,outputLater,0.01);
    }

}


int main() {
    //Zad1();
    //Zad2();
    Zad3();
    return 0;
}
