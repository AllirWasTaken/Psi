#include <iostream>
#include "NeuralNetwork.h"
#include "ActivationFunctions.h"
#include <cstdint>
#include <chrono>
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


void LoadSerialDataFromFile(const char* filename,Matrix &input,Matrix&target){


    std::vector<std::vector<float>> serialInput;
    std::vector<std::vector<float>> serialTarget;
    FILE*f = fopen(filename,"rt");
    if(!f)return;

    float t1,t2,t3;
    int t4;
    while(true) {

        if (fscanf(f, "%f %f %f %d",&t1,&t2,&t3,&t4) !=4) {
            break;
        }
        serialInput.push_back({t1,t2,t3});
        std::vector<float> tempTarget={0,0,0,0};
        tempTarget[t4-1]=1;
        serialTarget.push_back(tempTarget);

    }

    input=serialInput;
    input=input.Transpose();

    target=serialTarget;
    target=target.Transpose();
}





int main() {

    //Zad1
    {
        printf("Zad1\n");
        Matrix layer1,layer2,input,result;

        layer1={
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        layer2={
                {0.7,  0.9, -0.4, 0.8, 0.1},
                {0.8,  0.5, 0.3, 0.1, 0.0},
                {-0.3,  0.9, 0.3, 0.1, -0.2}
        };
        input={
                {0.5,0.1,0.2,0.8},
                {0.75,0.3,0.1,0.9},
                {0.1,0.7,0.6,0.2}
        };
        NeuralNetwork network;
        network.AddLayer(layer1,
                         ActivationFunctions::RectifiedLinearUnit,
                         ActivationFunctions::RectifiedLinearUnitDer);

        network.AddLayer(layer2);

        result=network.Run(input);
        std::cout<<result;


    }
    //Zad2
    {
        printf("Zad2\n");
        Matrix layer1,layer2,input,result,expected;

        layer1={
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        layer2={
                {0.7,  0.9, -0.4, 0.8, 0.1},
                {0.8,  0.5, 0.3, 0.1, 0.0},
                {-0.3,  0.9, 0.3, 0.1, -0.2}
        };
        input={
                {0.5,0.1,0.2,0.8},
                {0.75,0.3,0.1,0.9},
                {0.1,0.7,0.6,0.2}
        };
        expected={
                {0.1,0.5,0.1,0.7},
                {1.0,0.2,0.3,0.6},
                {0.1,-0.5,0.2,0.2}
        };

        double alpha=0.01;

        NeuralNetwork network;
        network.AddLayer(layer1,
                         ActivationFunctions::RectifiedLinearUnit,
                         ActivationFunctions::RectifiedLinearUnitDer);

        network.AddLayer(layer2);

        printf("Epoka 1:\n");
        result=network.Run(input);
        std::cout<<result;
        for(int i=0;i<49;i++)network.Update(input,expected,alpha);
        printf("Epoka 50:\n");
        result=network.Run(input);
        std::cout<<result;
    }
    //Zad3
    {
        printf("Zad3\n");

        Matrix trainInput,trainExpected,testInput,testExpected;

        LoadDataBase("../train-labels.idx1-ubyte", "../train-images.idx3-ubyte", trainInput, trainExpected);
        LoadDataBase("../t10k-labels.idx1-ubyte", "../t10k-images.idx3-ubyte", testInput, testExpected);

        NeuralNetwork imageNumberRecognition;
        //Do it only to initialize network


        Matrix inputLayer(784,40);
        Matrix outputLayer(40,10);

        inputLayer.Randomize(-0.1,0.1);
        outputLayer.Randomize(-0.1,0.1);

        imageNumberRecognition.AddLayer(inputLayer,ActivationFunctions::RectifiedLinearUnit,ActivationFunctions::RectifiedLinearUnitDer);
        imageNumberRecognition.AddLayer(outputLayer);


       //imageNumberRecognition.LoadFromFile("../imageNumberRecognition.ai");
        float targetScore=90.0f;
        int updatesPerTest=1;
        bool Train= true;
        bool Test = true;
        float percentage=100;
       while(true) {
           for (int i = 0; i < updatesPerTest&&Train; i++) {
               imageNumberRecognition.Update(trainInput, trainExpected, 0.01);
           }
           if(Test) {
               unsigned right = imageNumberRecognition.TestAndScore(testInput, testExpected);
               percentage = (float) right / (float)testInput.Width() * 100;
               std::cout << "Test score: " << right << "/" << testInput.Width() << " " << percentage << "%" << '\n';
           }
           //if(Train)imageNumberRecognition.SaveToFile("../imageNumberRecognition.ai");
           if(percentage>=targetScore||!Train)break;
       }






    }

    //Zad4
    {
        printf("Zad4\n");
        Matrix inputTrain;
        Matrix targetTrain;

        Matrix inputTest;
        Matrix targetTest;

        LoadSerialDataFromFile("../testData.txt",inputTest,targetTest);
        LoadSerialDataFromFile("../trainData.txt",inputTrain,targetTrain);

        NeuralNetwork colorRecognition;
        Matrix inputLayer(3,16);
        Matrix outputLayer(16,4);
        inputLayer.Randomize();
        outputLayer.Randomize();
        colorRecognition.AddLayer(inputLayer,ActivationFunctions::RectifiedLinearUnit,ActivationFunctions::RectifiedLinearUnitDer);
        colorRecognition.AddLayer(outputLayer);




        int hardening=5;

        for(int i=0; true;i++){
            printf("Skutecznosc po %d epokach: ",i+1);
            colorRecognition.Update(inputTrain, targetTrain, 0.01);
            int testResult= colorRecognition.TestAndScore(inputTest, targetTest);
            printf("%d/%d   %.02f%\n",testResult,inputTest.Width(),(float)testResult/(float)inputTest.Width()*100.0f);
            if(testResult==inputTest.Width()){
                hardening--;
                if(!hardening){
                    printf("Siec zostala nauczona\n");
                    break;
                }
            }
        }
    }
    return 0;

}


