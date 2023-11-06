#include "NeuronLayer.h"
#include <stdexcept>
#include <ctime>


PSI::NeuronLayer::NeuronLayer(int layerSize, int inputSize,
                              double(*activationFunction)(double),double(*activationDevFunc)(double)) {
    weightsMatrix.resize(layerSize);
    for (int i = 0; i <layerSize ; ++i) {
        weightsMatrix[i].resize(inputSize);
    }
    biasVector.resize(layerSize);
    inputCount=inputSize;
    neuronCount=layerSize;
    funcA=activationFunction;
    funcDev=activationDevFunc;
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

    if(funcA){
       for(int i=0;i<result.size();i++){
           result[i]=funcA(result[i]);
       }
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



double PSI::NeuronLayer::UpdateLayerOnce(const std::vector<std::vector<double>> &serialInput, const std::vector<std::vector<double>> &serialTarget,double alpha) {
    if(alpha<0||alpha>1){
        throw std::invalid_argument("Alpha is negative or above one");
    }
    if(serialInput.size()!=serialTarget.size()){
        throw std::invalid_argument("Input series size is not matching target series size");
    }
    double errResult=0;
    for(int i=0;i<serialInput.size();i++){
        //
        double serialError=0;
        if(serialTarget[i].size()!=neuronCount){
            throw std::invalid_argument("Target size is not matching output size");
        }
        std::vector<double> runResult= RunLayer(serialInput[i]);
        std::vector<std::vector<double>> errorMatrix;

        std::vector<double> deltaTemp;

        for(int j=0;j<neuronCount;j++){
            double temp=runResult[j]-serialTarget[i][j];
            serialError+=temp*temp;
            deltaTemp.push_back(2.0f / (double)neuronCount * temp);
        }
        errResult+=serialError/neuronCount;

        errorMatrix.resize(neuronCount);
        for(int z=0;z<neuronCount;z++){
            for(int j=0;j<inputCount;j++){
                errorMatrix[z].push_back(deltaTemp[z] * serialInput[i][j]);
            }
        }
        //

        for(int y=0;y<neuronCount;y++){
            for(int x=0;x<inputCount;x++){
                weightsMatrix[y][x]-=alpha*errorMatrix[y][x];
            }
        }
    }


    return errResult;
}

double PSI::NeuronLayer::TeachLayer(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget, double alpha, unsigned eraCount) {
    double errResult;
    for(int i=0;i<eraCount;i++){
        errResult=UpdateLayerOnce(serialInput,serialTarget,alpha);
    }
    return errResult;
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

void PSI::NeuronLayer::SetCalculateError(bool calcError) {
    errorCalc=calcError;
}