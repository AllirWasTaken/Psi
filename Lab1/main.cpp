#include <iostream>
#include "NeuronLayer.h"
#include "Neuron.hpp"





int main(){

//zad 1

    float input[]{
        0.75f,0.32,0.53,0.98,0.43
    };
    float bias=0.53;


    PSI::Neuron neuron(5,bias);
    neuron.Randomize();


    float wynik=neuron.NeuronRun(input);
    std::cout<<"zad1\n"<<wynik<<std::endl;

//zad2

    PSI::NeuronLayer layer(5,3);
    //layer.RandomizeLayer();

    std::vector<float> inputData={0.5,0.75,0.1};
    std::vector<std::vector<float>> weightMatrix={
            {0.1,0.1,-0.3},
            {0.1,0.2,0.0},
            {0.0,0.7,0.1},
            {0.2,0.4,0.0},
            {-0.3,0.5,0.1}
    };
    layer.SetWeightMatrix(weightMatrix);


    std::vector<float> outputTab=layer.RunLayer(inputData);

    std::cout<<"Zad2\n";
    for(int i=0;i<outputTab.size();i++){
        std::cout<<outputTab[i]<<" ";
    }

    std::cout<<std::endl;


    return 0;
};


