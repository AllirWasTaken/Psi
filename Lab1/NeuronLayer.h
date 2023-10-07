#include <vector>



namespace PSI {

    class NeuronLayer {
    public:
        NeuronLayer(int layerSize,int inputSize);
        std::vector <float> RunLayer(std::vector <float> &input);
        void SetWeightMatrix(std::vector <std::vector<float>> &input);
        void RandomizeLayer();



    private:
        std::vector<std::vector<float>> weightsMatrix;
        std::vector<float> biasVector;
        int neuronCount;
        int inputCount;


    };
}



