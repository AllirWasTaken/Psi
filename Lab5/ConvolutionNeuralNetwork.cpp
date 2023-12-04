#include "ConvolutionNeuralNetwork.h"


using namespace ALib;

void ConvolutionNeuralNetwork::AddFullyConnected(int inputs, int neurons, void (*aFunc)(ALib::Matrix &),
                                                 void (*aFuncDev)(ALib::Matrix &)) {
    Matrix newMatrix(inputs,neurons);
    layers.push_back(newMatrix);
    typeOfLayer.push_back(FullyConnected);
    pooling.push_back(0);
    activationFunctions.push_back(aFunc);
    devActivationFunctions.push_back(aFuncDev);

}

void
ConvolutionNeuralNetwork::AddConvolution(int filterX, int filterY, int filterCount, int inputX, int inputY, int pooling,
                                         void (*aFunc)(ALib::Matrix &), void (*aFuncDev)(ALib::Matrix &)) {

    
    typeOfLayer.push_back(Convolution);
    this->pooling.push_back(pooling);
    activationFunctions.push_back(aFunc);
    devActivationFunctions.push_back(aFuncDev);
}