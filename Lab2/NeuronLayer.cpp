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


std::vector<double> PSI::NeuronLayer::RunLayer(const std::vector<double> &input) {

    if(input.size()!=inputCount){
        throw std::invalid_argument("Wrong size of input vector");
    }


    std::vector<double> result;


    for(int i=0;i<neuronCount;i++){
        double sum=0;
        for(int j=0;j<inputCount;j++){
            sum+=input[j]*weightsMatrix[i][j];
        }
        result.push_back(sum);
    }

    return result;

}


void PSI::NeuronLayer::RandomizeLayer(double min,double max) {
    if(max<=min){
        throw std::invalid_argument("Max has to be bigger value than min");
    }
    srand(time(nullptr));
    for(int i=0;i<neuronCount;i++){
        for (int j = 0; j < inputCount; ++j) {
            weightsMatrix[i][j]=(double)((rand()/(double)RAND_MAX)*(max-min)+min);
        }
    }

}

void PSI::NeuronLayer::SetWeightMatrix(std::vector<std::vector<double>> &input) {
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

    std::vector<std::vector<double>> newMatrix;
    newMatrix.resize(neurons);
    for(int i=0;i<neurons;i++) {
        for(int j=0;j<inputs;j++) {
            double temp;
            if (fscanf(f, "%f", &temp) == EOF) {
                throw std::invalid_argument("File is corrupted or un matching");
            }
            newMatrix[i].push_back(temp);
        }
    }
    SetWeightMatrix(newMatrix);
}

std::vector<std::vector<double>> PSI::NeuronLayer::GetWeightErrorDiff(const std::vector<double> &input, const std::vector<double> &target) {
    if(target.size()!=neuronCount){
        throw std::invalid_argument("Target size is not matching output size");
    }
    std::vector<double> runResult= RunLayer(input);
    std::vector<std::vector<double>> result;

    std::vector<double> deltaTemp;
    for(int i=0;i<neuronCount;i++){
        deltaTemp.push_back(2.0f * 1.0f / (double)neuronCount * (runResult[i]-target[i]));
    }

    result.resize(neuronCount);
    for(int i=0;i<neuronCount;i++){
        for(int j=0;j<inputCount;j++){
            result[i].push_back(deltaTemp[i]*input[j]);
        }
    }

    return result;
}

void PSI::NeuronLayer::UpdateLayerOnce(const std::vector<std::vector<double>> &serialInput, const std::vector<std::vector<double>> &serialTarget,double alpha) {
    if(alpha<0||alpha>1){
        throw std::invalid_argument("Alpha is negative or above one");
    }
    if(serialInput.size()!=serialTarget.size()){
        throw std::invalid_argument("Input series size is not matching target series size");
    }
for(int i=0;i<serialInput.size();i++){
    std::vector<std::vector<double>> errorMatrix= GetWeightErrorDiff(serialInput[i],serialTarget[i]);
    for(int y=0;y<neuronCount;y++){
        for(int x=0;x<inputCount;x++){
            weightsMatrix[y][x]-=alpha*errorMatrix[y][x];
        }
    }
}



}

void PSI::NeuronLayer::TeachLayer(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,unsigned int eraCount, double alpha) {
    for(int i=0;i<eraCount;i++){
        UpdateLayerOnce(serialInput,serialTarget,alpha);
    }
}

double PSI::NeuronLayer::TeachAndGetError(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,double alpha) {
    double result=0;

    if(alpha<0||alpha>1){
        throw std::invalid_argument("Alpha is negative or above one");
    }
    if(serialInput.size()!=serialTarget.size()){
        throw std::invalid_argument("Input series size is not matching target series size");
    }
    for(int j=0; j < serialInput.size(); j++){

        std::vector<double> output= RunLayer(serialInput[j]);
        double temp=0;
        for(int i=0;i<neuronCount;i++){
            temp+=(output[i]-serialTarget[j][i])*(output[i]-serialTarget[j][i]);
        }
        temp/=(double)neuronCount;
        result+=temp;

        std::vector<std::vector<double>> errorMatrix= GetWeightErrorDiff(serialInput[j], serialTarget[j]);
        for(int y=0;y<neuronCount;y++){
            for(int x=0;x<inputCount;x++){
                weightsMatrix[y][x]-=alpha*errorMatrix[y][x];
            }
        }
    }

    return result;
}

unsigned PSI::NeuronLayer::TestGrid(const std::vector<std::vector<double>> &serialInput,
                                    const std::vector<std::vector<double>> &serialTarget) {
    if(serialInput.size()!=serialTarget.size()){
        throw std::invalid_argument("Input series size is not matching target series size");
    }
    unsigned result=0;
    for(int j=0;j<serialInput.size();j++){
        std::vector<double> output= RunLayer(serialInput[j]);
        int mO=0,mT=0;
        double mvO=output[0],mvT=serialTarget[j][0];
        for(int i=0;i<output.size();i++){
            if(output[i]>mvO){
                mvO=output[i];
                mO=i;
            }
            if(serialTarget[j][i]>mvT){
                mvT=output[i];
                mT=i;
            }
        }
        if(mO==mT)result++;
    }

    return result;
}