#ifndef LAB1_NEURONGRID_H
#define LAB1_NEURONGRID_H
#include "NeuronLayer.h"


namespace PSI {

    class NeuronGrid {
    public:
        std::vector<float> RunGrid(std::vector<float> &input);
        void AddLayerOnBack(unsigned size);
        NeuronLayer &GetLayer(unsigned layerNumber);
        NeuronGrid(unsigned firstLayerInputCount,unsigned firstLayerNeuronCount);


    private:
        std::vector<NeuronLayer> layers;

    };
}

#endif //LAB1_NEURONGRID_H
