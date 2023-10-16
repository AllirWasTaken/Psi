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

std::vector<double> PSI::NeuronGrid::RunGrid(std::vector<double> &input) {
    if(input.size()!=layers[0].GetInputCount()){
        throw std::invalid_argument("Invalid input vector size");
    }
    std::vector<double> result=input;

    for(int i=0;i<layers.size();i++){
        result=layers[i].RunLayer(result);
    }
    return result;
}