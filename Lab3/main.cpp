#include <iostream>
#include "NeuralNetwork.h"
#include "ActivationFunctions.h"

using namespace ALib;
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
        network.SaveToFile("../CoAha");
    }
    //Zad3
    {
        printf("Zad3\n");

    }
    return 0;

}


