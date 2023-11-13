#include <cstdint>
#include "NeuralNetwork.h"
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

    //labelCount=1000;

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

int main(){
    //Zad1
    {
        printf("Zad1\n");

        Matrix trainInput,trainExpected,testInput,testExpected;

        LoadDataBase("../train-labels.idx1-ubyte", "../train-images.idx3-ubyte", trainInput, trainExpected);
        LoadDataBase("../t10k-labels.idx1-ubyte", "../t10k-images.idx3-ubyte", testInput, testExpected);

        NeuralNetwork imageNumberRecognition;
        //Do it only to initialize network
        /*
        Matrix inputLayer(784,100);
        Matrix outputLayer(100,10);

        inputLayer.Randomize(-0.1,0.1);
        outputLayer.Randomize(-0.1,0.1);

        imageNumberRecognition.AddLayer(inputLayer,ActivationFunctions::RectifiedLinearUnit,ActivationFunctions::RectifiedLinearUnitDer);
        imageNumberRecognition.AddLayer(outputLayer);
        */

       imageNumberRecognition.LoadFromFile("../60000data100neurons50dropOut.ai");
        float targetScore=98.0f;
        int updatesPerTest=1;
        bool Train= false;
        bool Test = false;
        float percentage=100;
       while(true) {
           for (int i = 0; i < updatesPerTest&&Train; i++) {
               imageNumberRecognition.Update(trainInput, trainExpected, 0.005,0.5);
           }
           if(Test) {
               unsigned right = imageNumberRecognition.TestAndScore(testInput, testExpected);
               percentage = (float) right / (float)testInput.Width() * 100;
               std::cout << "Test score: " << right << "/" << testInput.Width() << " " << percentage << "%" << '\n';
           }
           if(Train)imageNumberRecognition.SaveToFile("../60000data100neurons50dropOut.ai");
           if(percentage>=targetScore||!Train)break;
       }






    }
    //Zad2
    {
        printf("Zad2\n");


        Matrix trainInput,trainExpected,testInput,testExpected;

        LoadDataBase("../train-labels.idx1-ubyte", "../train-images.idx3-ubyte", trainInput, trainExpected);
        LoadDataBase("../t10k-labels.idx1-ubyte", "../t10k-images.idx3-ubyte", testInput, testExpected);

        NeuralNetwork imageNumberRecognition;
        //Do it only to initialize network

        Matrix inputLayer(784,100);
        Matrix outputLayer(100,10);

        inputLayer.Randomize(-0.01,0.01);
        outputLayer.Randomize(-0.01,0.01);

        imageNumberRecognition.AddLayer(inputLayer,ActivationFunctions::RectifiedLinearUnit,ActivationFunctions::RectifiedLinearUnitDer);
        imageNumberRecognition.AddLayer(outputLayer);


        //imageNumberRecognition.LoadFromFile("../100batching60000data100neurons50dropOut.ai");
        float targetScore=95.0f;
        int updatesPerTest=1;
        bool Train= true;
        bool Test = true;
        float percentage=100;
        while(true) {
            if(Train){
                imageNumberRecognition.UpdateMiniBatch(trainInput, trainExpected, 0.02,
                                                       100, updatesPerTest, 0.5);
            }
            if(Test) {
                unsigned right = imageNumberRecognition.TestAndScore(testInput, testExpected);
                percentage = (float) right / (float)testInput.Width() * 100;
                std::cout << "Test score: " << right << "/" << testInput.Width() << " " << percentage << "%" << '\n';
            }
            if(Train)imageNumberRecognition.SaveToFile("../100batching60000data100neurons50dropOut.ai");
            if(percentage>=targetScore||!Train)break;
        }


    }



    return 0;
}