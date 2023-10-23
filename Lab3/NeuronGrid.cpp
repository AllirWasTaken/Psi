#include "NeuronGrid.h"
#include <stdexcept>

PSI::NeuronLayer &PSI::NeuronGrid::GetLayer(unsigned int layerNumber) {
    if(layerNumber>=layers.size()){
        throw std::invalid_argument("Layer number is exceeding layer count");
    }
    return layers[layerNumber];
}

void PSI::NeuronGrid::AddLayerOnBack(unsigned int size) {
    NeuronLayer newLayer((int)size,(int)layers[layers.size()-1].GetNeuronCount());
    layers.push_back(newLayer);
}

PSI::NeuronGrid::NeuronGrid(unsigned int firstLayerInputCount, unsigned int firstLayerNeuronCount) {
    NeuronLayer newLayer((int)firstLayerNeuronCount,(int)firstLayerInputCount);
    layers.push_back(newLayer);
}

std::vector<double> PSI::NeuronGrid::RunGrid(const std::vector<double> &input) {
    if(input.size()!=layers[0].GetInputCount()){
        throw std::invalid_argument("Invalid input vector size");
    }
    std::vector<double> result=input;

    for(int i=0;i<layers.size();i++){
        result=layers[i].RunLayer(result);
    }
    return result;
}

double PSI::NeuronGrid::TeachGrid(const std::vector<std::vector<double>> &serialInput,
                                  const std::vector<std::vector<double>> &serialTarget,
                                  double(*activationFunc)(double),double alpha,
                                  unsigned int eraCount) {
    double errResult;
    for(int i=0;i<eraCount;i++){
        errResult= UpdateGridOnce(serialInput,serialTarget,activationFunc,alpha);
    }
    return errResult;
}

double PSI::NeuronGrid::UpdateGridOnce(const std::vector<std::vector<double>> &serialInput,
                                       const std::vector<std::vector<double>> &serialTarget,
                                       double(*activationFunc)(double),double alpha) {
    if(serialInput.size()!=serialTarget.size()){
        throw std::invalid_argument("Input series size is not matching target series size");
    }
    double errResult=0;

    for(int seriesNumber=0;seriesNumber<serialTarget.size();seriesNumber++){
        std::vector<std::vector<double>> layersOutput;
        layersOutput.resize(layers.size());

        std::vector<double> currentInput=serialInput[seriesNumber];

        for(int currentLayer=0;currentLayer<layers.size();currentLayer++){
            layersOutput[currentLayer]=layers[currentLayer].RunLayer(currentInput);
            currentInput=layersOutput[currentLayer];
        }

        for(int currentLayer=0;currentLayer<layers.size();currentLayer++) {

        }

    }








    return errResult;
}
