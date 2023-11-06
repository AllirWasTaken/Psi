#include "Matrix.h"



namespace PSI {

    class NeuronLayer {
    public:
        NeuronLayer(int neuronCount,int inputSize,double(*activationFunc)(double)=nullptr,double(*activationDevFunc)(double)=nullptr);
        std::vector <double> RunLayer(const std::vector <double> &input);
        void SetWeightMatrix(std::vector <std::vector<double>> &input);
        unsigned GetNeuronCount();
        unsigned GetInputCount();
        void RandomizeLayer(double min=0,double max=1);
        void LoadWeightsFromFile(const char* fileName);
        double TeachLayer(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,double alpha,unsigned eraCount);
        double UpdateLayerOnce(const std::vector<std::vector<double>> &serialInput,const std::vector<std::vector<double>> &serialTarget,double alpha);
        unsigned TestGrid(const std::vector<std::vector<double>> &serialInput, const std::vector<std::vector<double>> &serialTarget);
        void SetCalculateError(bool calcError);


    private:
        std::vector<std::vector<double>> weightsMatrix;
        std::vector<double> biasVector;
        int neuronCount;
        int inputCount;
        double(*funcA)(double);
        double(*funcDev)(double);
        bool errorCalc=false;
    };
}



