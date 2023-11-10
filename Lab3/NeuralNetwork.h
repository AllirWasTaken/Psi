#ifndef PSI_NEURAL_NETWORK_H
#define PSI_NEURAL_NETWORK_H
#include "Matrix.h"
class NeuralNetwork {
private:
    std::vector<ALib::Matrix> network;
    std::vector<void(*)(ALib::Matrix&)> activationFunctions;
    std::vector<void(*)(ALib::Matrix&)> devActivationFunctions;
public:
    NeuralNetwork()=default;
    void AddLayer(ALib::Matrix &newWeightMatrix,void(*aFunc)(ALib::Matrix&)=nullptr,void(*aFuncDev)(ALib::Matrix&)=nullptr);
    void AddRandomLayer(unsigned neuronCount,void(*aFunc)(ALib::Matrix&)=nullptr,void(*aFuncDev)(ALib::Matrix&)=nullptr);
    ALib::Matrix Run(const ALib::Matrix &input);
    void Update(const ALib::Matrix &input, const ALib::Matrix &expected,double alpha);
    void SaveToFile(const char* fileName);
    void LoadFromFile(const char* fileName);
};
#endif //PSI_NEURAL_NETWORK_H
