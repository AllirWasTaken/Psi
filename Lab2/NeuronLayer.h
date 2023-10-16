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
        void TeachLayer(const std::vector<std::vector<float>> &serialInput,const std::vector<std::vector<float>> &serialTarget,unsigned eraCount,float alpha);
        void UpdateLayerOnce(const std::vector<std::vector<float>> &serialInput,const std::vector<std::vector<float>> &serialTarget,float alpha);
        double GetError(const std::vector<std::vector<float>> &serialInput, const std::vector<std::vector<float>> &serialTarget);

    private:
        std::vector<std::vector<float>>  GetWeightErrorDiff(const std::vector<float> &input,const std::vector<float> &target);
        std::vector<std::vector<float>> weightsMatrix;
        std::vector<float> biasVector;
        int neuronCount;
        int inputCount;


    };
}



