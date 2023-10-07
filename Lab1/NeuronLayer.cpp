#include "NeuronLayer.h"
#include <random>
#include <stdexcept>
#include <ctime>


PSI::NeuronLayer::NeuronLayer(int layerSize, int inputSize) {
    weightsMatrix.resize(layerSize);
    for (int i = 0; i <layerSize ; ++i) {
        weightsMatrix[i].resize(inputSize);
    }
    biasVector.resize(5);
    inputCount=inputSize;
    neuronCount=layerSize;

}


std::vector<float> PSI::NeuronLayer::RunLayer(std::vector<float> &input) {

    if(input.size()!=inputCount){
        throw std::invalid_argument("Wrong size of input vector");
    }


    std::vector<float> result;


    for(int i=0;i<neuronCount;i++){
        float sum=0;
        for(int j=0;j<inputCount;j++){
            sum+=input[j]*weightsMatrix[i][j];
        }
        result.push_back(sum);
    }

    return result;

}


void PSI::NeuronLayer::RandomizeLayer() {
    srand(time(NULL));
    for(int i=0;i<neuronCount;i++){
        for (int j = 0; j < inputCount; ++j) {
            weightsMatrix[i][j]=(float)(rand()%201-100)/100.0f;
        }
    }

    for(int i=0;i<neuronCount;i++){
        biasVector[i]=(float)(rand()%201-100)/100.0f;
    }



}

void PSI::NeuronLayer::SetWeightMatrix(std::vector<std::vector<float>> &input) {
    if(input.size()!=neuronCount){
        throw std::invalid_argument("Not matching matrix size");
    }
    for (int i = 0; i < neuronCount; i++) {
        if(input[i].size()!=inputCount) {
            throw std::invalid_argument("Not matching matrix size");
        }
    }
    weightsMatrix=input;
}