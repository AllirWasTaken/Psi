#ifndef LAB1_NEURONGRID_H
#define LAB1_NEURONGRID_H
#include "NeuronLayer.h"


namespace PSI {

    class NeuronGrid {
    public:
        std::vector<double> RunGrid(const std::vector<double> &input);
        void AddLayerOnBack(unsigned size);
        NeuronLayer &GetLayer(unsigned layerNumber);
        NeuronGrid(unsigned firstLayerInputCount,unsigned firstLayerNeuronCount);
        double UpdateGridOnce(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,double(*activationFunc)(double),double alpha);
        double TeachGrid(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,double(*activationFunc)(double),double alpha,unsigned eraCount);


    private:
        std::vector<NeuronLayer> layers;

    };
}

#endif //LAB1_NEURONGRID_H
