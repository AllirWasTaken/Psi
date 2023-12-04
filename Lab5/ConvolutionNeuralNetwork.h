#ifndef LAB5_CONVOLUTIONNEURALNETWORK_H
#define LAB5_CONVOLUTIONNEURALNETWORK_H

#include "Matrix.h"
#include <vector>



enum LayerType{
    Convolution,
    FullyConnected
};


class ConvolutionNeuralNetwork {
private:
    std::vector<ALib::Matrix> layers;
    std::vector<LayerType> typeOfLayer;
    std::vector<int> pooling;
    std::vector<void(*)(ALib::Matrix&)> activationFunctions;
    std::vector<void(*)(ALib::Matrix&)> devActivationFunctions;




public:
    void AddConvolution(int filterX,int filterY,int filterCount,int inputX,int inputY, int pooling
        ,void(*aFunc)(ALib::Matrix&)=nullptr,void(*aFuncDev)(ALib::Matrix&)=nullptr);
    void AddFullyConnected(int inputs, int neurons
        , void(*aFunc)(ALib::Matrix &)=nullptr, void(*aFuncDev)(ALib::Matrix &)=nullptr);
    ALib::Matrix Run(ALib::Matrix &input);
    ALib::Matrix Update(ALib::Matrix &input,ALib::Matrix &expected,float alpha);










};


#endif //LAB5_CONVOLUTIONNEURALNETWORK_H
