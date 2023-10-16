#include "NeuronLayer.h"
#include <iostream>



int main(){

    //Zad1
    {
        PSI::NeuronLayer singleNeuron(1,1);
        std::vector<std::vector<float>> weight={{0.5}};
        singleNeuron.SetWeightMatrix(weight);
        std::vector<std::vector<float>> input={{2}};
        std::vector<std::vector<float>> target={{0.8}};
        float alpha=0.1;

        printf("Zad1\n");
        singleNeuron.TeachLayer(input,target,5,alpha);
        printf("Error po 5: %.20lf\n",singleNeuron.GetError(input,target));
        singleNeuron.TeachLayer(input,target,15,alpha);
        printf("Error po 20: %.20lf\n",singleNeuron.GetError(input,target));


    }



    //Zad2
    {
        PSI::NeuronLayer layer(5, 3);

        std::vector<std::vector<float>> input = {
                {0.5, 0.75, 0.1},
                {0.1,0.3,0.7},
                {0.2,0.1,0.6},
                {0.8,0.9,0.2}
        };
        std::vector<std::vector<float>> target = {
                {0.1, 1, 0.1, 0, -0.1},
                {0.5,0.2,-0.5,0.3,0.7},
                {0.1,0.3,0.2,0.9,0.1},
                {0.7,0.6,0.2,-0.1,0.8}
        };
        std::vector<std::vector<float>> weightMatrix = {
                {0.1,  0.1, -0.3},
                {0.1,  0.2, 0.0},
                {0.0,  0.7, 0.1},
                {0.2,  0.4, 0.0},
                {-0.3, 0.5, 0.1}
        };
        layer.SetWeightMatrix(weightMatrix);
        printf("Zad2\n");

        layer.TeachLayer(input,target,1000,0.01);

        printf("Error po 1000 epokach: %lf\n",layer.GetError(input,target));

    }
    //Zad3
    {
        


    }

    return 0;

}


