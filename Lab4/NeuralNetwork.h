#ifndef PSI_NEURAL_NETWORK_H
#define PSI_NEURAL_NETWORK_H
#include "ActivationFunctions.h"

class NeuralNetwork {
private:
    std::vector<ALib::Matrix> network;
    std::vector<void(*)(ALib::Matrix&)> activationFunctions;
    std::vector<void(*)(ALib::Matrix&)> devActivationFunctions;
    static ALib::Matrix GetDropOutMatrix(ALib::Matrix& matrixFor, float dropOut);
    static void BatchedUpdateWeightDiff(BatchedThreadingArgs *args);
public:
    NeuralNetwork()=default;
    void AddLayer(ALib::Matrix &newWeightMatrix,void(*aFunc)(ALib::Matrix&)=nullptr,void(*aFuncDev)(ALib::Matrix&)=nullptr);
    void AddRandomLayer(unsigned neuronCount,void(*aFunc)(ALib::Matrix&)=nullptr,void(*aFuncDev)(ALib::Matrix&)=nullptr);
    ALib::Matrix Run(const ALib::Matrix &input);
    void Update(const ALib::Matrix &input, const ALib::Matrix &expected,float alpha, float dropOut=0);
    void SaveToFile(const char* fileName);
    void LoadFromFile(const char* fileName);
    unsigned TestAndScore(const ALib::Matrix &input, const ALib::Matrix &expected);
    void UpdateMiniBatch(const ALib::Matrix &input, const ALib::Matrix &expected, float alpha, unsigned batchSize, unsigned iterationsCount, float dropOut=0);
};


#endif //PSI_NEURAL_NETWORK_H
