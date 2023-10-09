#include <vector>



namespace PSI {

    class NeuronLayer {
    public:
        NeuronLayer(int neuronCount,int inputSize);
        std::vector <float> RunLayer(const std::vector <float> &input);
        void SetWeightMatrix(std::vector <std::vector<float>> &input);
        unsigned GetNeuronCount();
        unsigned GetInputCount();
        void RandomizeLayer(float min=0,float max=1);
        void LoadWeightsFromFile(const char* fileName);
        std::vector<std::vector<float>>  GetWeightErrorDiff(const std::vector<float> &input,const std::vector<float> &target);


    private:
        std::vector<std::vector<float>> weightsMatrix;
        std::vector<float> biasVector;
        int neuronCount;
        int inputCount;


    };
}



