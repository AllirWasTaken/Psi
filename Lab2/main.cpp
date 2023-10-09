#include "NeuronLayer.h"




int main(){

    PSI::NeuronLayer layer(5,3);

    std::vector<float> input={0.5,0.75,0.1};
    std::vector<float> target={0.1,1,0.1,0,-0.1};
    std::vector<std::vector<float>> weightMatrix={
            {0.1,  0.1, -0.3},
            {0.1,  0.2, 0.0},
            {0.0,  0.7, 0.1},
            {0.2,  0.4, 0.0},
            {-0.3, 0.5, 0.1}
    };
    layer.SetWeightMatrix(weightMatrix);

    std::vector<std::vector<float>> errorResult=layer.GetWeightErrorDiff(input,target);


    return 0;

}


