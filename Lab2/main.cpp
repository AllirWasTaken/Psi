#include "NeuronLayer.h"
#include <iostream>


void LoadSerialDataFromFile(const char* filename,std::vector<std::vector<double>> &serialInput,std::vector<std::vector<double>> &serialTarget){

    FILE*f = fopen(filename,"rt");
    if(!f)return;

    float t1,t2,t3;
    int t4;
    while(true) {

        if (fscanf(f, "%f %f %f %d",&t1,&t2,&t3,&t4) !=4) {
            break;
        }
        serialInput.push_back({t1,t2,t3});
        std::vector<double> tempTarget={0,0,0,0};
        tempTarget[t4-1]=1;
        serialTarget.push_back(tempTarget);

    }


}



int main(){

    //Zad1
    {
        PSI::NeuronLayer singleNeuron(1,1);
        std::vector<std::vector<double>> weight={{0.5}};
        singleNeuron.SetWeightMatrix(weight);
        std::vector<std::vector<double>> input={{2}};
        std::vector<std::vector<double>> target={{0.8}};
        double alpha=0.1;

        printf("Zad1\n");
        printf("Error po 5: %.40lf\n", singleNeuron.TeachLayer(input, target,5,alpha));
        printf("Error po 20: %.40lf\n", singleNeuron.TeachLayer(input, target,15,alpha));


    }



    //Zad2
    {
        PSI::NeuronLayer layer(5, 3);

        std::vector<std::vector<double>> input = {
                {0.5, 0.75, 0.1},
                {0.1,0.3,0.7},
                {0.2,0.1,0.6},
                {0.8,0.9,0.2}
        };
        std::vector<std::vector<double>> target = {
                {0.1, 1, 0.1, 0, -0.1},
                {0.5,0.2,-0.5,0.3,0.7},
                {0.1,0.3,0.2,0.9,0.1},
                {0.7,0.6,0.2,-0.1,0.8}
        };
        std::vector<std::vector<double>> weightMatrix = {
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        layer.SetWeightMatrix(weightMatrix);
        printf("Zad2\n");



        printf("Error po 1000 epokach: %lf\n", layer.TeachLayer(input,target,1000,0.01));

    }
    //Zad3
    {
        std::vector<std::vector<double>> inputTrain;
        std::vector<std::vector<double>> targetTrain;

        std::vector<std::vector<double>> inputTest;
        std::vector<std::vector<double>> targetTest;

        LoadSerialDataFromFile("../testData.txt",inputTest,targetTest);
        LoadSerialDataFromFile("../trainData.txt",inputTrain,targetTrain);

        PSI::NeuronLayer maciek(4, 3);
        maciek.RandomizeLayer(-1, 1);

        printf("Zad3\n");

        int hardening=5;

        for(int i=0;i<1000;i++){
            printf("Skutecznosc po %d epokach: ",i+1);
            maciek.TeachLayer(inputTrain, targetTrain, 1, 0.01);
            int testResult=maciek.TestGrid(inputTest, targetTest);
            printf("%d/%d   %.02f%\n",testResult,inputTest.size(),(float)testResult/(float)inputTest.size()*100.0f);
            if(testResult==inputTest.size()){
                hardening--;
                if(!hardening){
                    printf("Siec zostala nauczona\n");
                    break;
                }
            }
        }
        std::vector<double> opowiedz=maciek.RunLayer(inputTest[1]);


    }

    return 0;

}


