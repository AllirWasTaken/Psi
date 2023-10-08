#include <iostream>
#include "Neuron.hpp"
#include "NeuronGrid.h"





int main(){

    //zad 1
    {

        float input[]{
                0.75f, 0.32, 0.53, 0.98, 0.43
        };
        float bias = 0.53;


        PSI::Neuron neuron(5, bias);
        neuron.Randomize();


        float wynik = neuron.NeuronRun(input);
        std::cout << "zad1\n" << wynik << std::endl;
    }
    //zad2
    {

        PSI::NeuronLayer layer(5, 3);

        std::vector<float> inputData = {0.5, 0.75, 0.1};
        std::vector<std::vector<float>> weightMatrix = {
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        layer.SetWeightMatrix(weightMatrix);


        std::vector<float> outputTab = layer.RunLayer(inputData);

        std::cout << "Zad2\n";
        for (int i = 0; i < outputTab.size(); i++) {
            std::cout << outputTab[i] << " ";
        }

        std::cout << std::endl;
    }
    //zad 3
    {


        std::vector<std::vector<float>> layer1={
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        std::vector<std::vector<float>> layer2={
                {0.7,  0.9, -0.4, 0.8, 0.1},
                {0.8,  0.5, 0.3, 0.1, 0.0},
                {-0.3,  0.9, 0.3, 0.1, -0.2}
        };

        std::vector<float> input={0.5,0.75,0.1};

        PSI::NeuronGrid neuronGrid(3,5);
        neuronGrid.GetLayer(0).SetWeightMatrix(layer1);

        neuronGrid.AddLayerOnBack(3);
        neuronGrid.GetLayer(1).SetWeightMatrix(layer2);

        std::vector<float> output=neuronGrid.RunGrid(input);

        std::cout<<"Zad 3\n";
        for(int i=0;i<output.size();i++){
            std::cout<<output[i]<<" ";
        }
        std::cout<<std::endl;




    }
    //Zad4
    {
        std::vector<float> input={0.2,0.5,0.7};
        PSI::NeuronGrid neuronGrid(3,4);

        neuronGrid.GetLayer(0).RandomizeLayer(-1,1);

        neuronGrid.AddLayerOnBack(2);

        neuronGrid.GetLayer(1).LoadWeightsFromFile("../weights.txt");

        std::vector<float> output=neuronGrid.RunGrid(input);

        std::cout<<"Zad 4\n";
        for(int i=0;i<output.size();i++){
            std::cout<<output[i]<<" ";
        }
        std::cout<<std::endl;

    }

    return 0;
};


