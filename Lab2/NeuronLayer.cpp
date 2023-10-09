#include "NeuronLayer.h"
#include <random>
#include <stdexcept>
#include <ctime>


PSI::NeuronLayer::NeuronLayer(int layerSize, int inputSize) {
    weightsMatrix.resize(layerSize);
    for (int i = 0; i <layerSize ; ++i) {
        weightsMatrix[i].resize(inputSize);
    }
    biasVector.resize(layerSize);
    inputCount=inputSize;
    neuronCount=layerSize;

}


std::vector<float> PSI::NeuronLayer::RunLayer(const std::vector<float> &input) {

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


void PSI::NeuronLayer::RandomizeLayer(float min,float max) {
    if(max<=min){
        throw std::invalid_argument("Max has to be bigger value than min");
    }
    srand(time(nullptr));
    for(int i=0;i<neuronCount;i++){
        for (int j = 0; j < inputCount; ++j) {
            weightsMatrix[i][j]=(float)(rand()%int((max+abs(min))*1000)-1000*abs(min))/1000.0f;
        }
    }

    for(int i=0;i<neuronCount;i++){
        biasVector[i]=(float)(rand()%int((max+abs(min))*1000)-1000*abs(min))/1000.0f;
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

unsigned PSI::NeuronLayer::GetNeuronCount() {
    return neuronCount;
}
unsigned PSI::NeuronLayer::GetInputCount() {
    return inputCount;
}

void PSI::NeuronLayer::LoadWeightsFromFile(const char *fileName) {

    FILE *f = fopen(fileName,"rt");
    if(!f){
        throw std::invalid_argument("File with given name cannot be opened");
    }

    int inputs,neurons;
    fscanf(f,"%d %d",&inputs,&neurons);
    if(inputs<1||neurons<1||inputs!=inputCount||neurons!=neuronCount){
        throw std::invalid_argument("Matrix sizes are not matching the layer sizes");
    }

    std::vector<std::vector<float>> newMatrix;
    newMatrix.resize(neurons);
    for(int i=0;i<neurons;i++) {
        for(int j=0;j<inputs;j++) {
            float temp;
            if (fscanf(f, "%f", &temp) == EOF) {
                throw std::invalid_argument("File is corrupted or un matching");
            }
            newMatrix[i].push_back(temp);
        }
    }
    SetWeightMatrix(newMatrix);
}

std::vector<std::vector<float>> PSI::NeuronLayer::GetWeightErrorDiff(const std::vector<float> &input, const std::vector<float> &target) {
    if(target.size()!=neuronCount){
        throw std::invalid_argument("Target size is not matching output size");
    }
    std::vector<float> runResult= RunLayer(input);
    std::vector<std::vector<float>> result;

    std::vector<float> deltaTemp;
    for(int i=0;i<neuronCount;i++){
        deltaTemp.push_back(2.0f * 1.0f / (float)neuronCount * (runResult[i]-target[i]));
    }

    result.resize(neuronCount);
    for(int i=0;i<neuronCount;i++){
        for(int j=0;j<inputCount;j++){
            result[i].push_back(deltaTemp[i]*input[j]);
        }
    }

    return result;
}
