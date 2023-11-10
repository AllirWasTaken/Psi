#include "NeuralNetwork.h"
#include "ActivationFunctions.h"
using namespace ALib;


void NeuralNetwork::AddLayer(ALib::Matrix &newWeightMatrix, void (*aFunc)(ALib::Matrix &),
                             void (*aFuncDev)(ALib::Matrix &)) {
    if(network.empty()){
        network.push_back(newWeightMatrix);
    }
    else{
        if(newWeightMatrix.Width()!=network[network.size()-1].Height()){
            throw std::invalid_argument("new matrix is not matching size with last layer");
        }
        network.push_back(newWeightMatrix);
    }
    activationFunctions.push_back(aFunc);
    devActivationFunctions.push_back(aFuncDev);
}

ALib::Matrix NeuralNetwork::Run(const ALib::Matrix &input) {
    Matrix result=input;
    for(int i=0;i<network.size();i++){
        result=network[i]*result;
        if(activationFunctions[i]){
            activationFunctions[i](result);
        }
    }
    return result;
}

void NeuralNetwork::AddRandomLayer(unsigned int neuronCount, void (*aFunc)(ALib::Matrix &),
                                   void (*aFuncDev)(ALib::Matrix &)) {
    if(neuronCount==0){
        throw std::invalid_argument("neuron count cannot be 0");
    }
    if(network.empty()){
        throw std::invalid_argument("Input layer cannot be random");
    }

    Matrix newMatrix(neuronCount,network[network.size()-1].Height());
    srand(time(nullptr));
    for(int y=0;y<newMatrix.Height();y++){
        for(int x=0;x<newMatrix.Width();x++){
            newMatrix[y][x]=rand()/RAND_MAX*20-10;
        }
    }

    network.push_back(newMatrix);
    activationFunctions.push_back(aFunc);
    devActivationFunctions.push_back(aFuncDev);
}

void NeuralNetwork::Update(const ALib::Matrix &input, const ALib::Matrix &expected, double alpha) {
    for(int serial=0;serial<input.Width();serial++){
        Matrix currentInput(1,input.Height());
        for(int y=0;y<input.Height();y++){
            currentInput[y][0]=input[y][serial];
        }
        Matrix currentExpected(1,expected.Height());
        for(int y=0;y<expected.Height();y++){
            currentExpected[y][0]=expected[y][serial];
        }

        std::vector<Matrix> layersOutputs;
        layersOutputs.resize(network.size());
        std::vector<Matrix> layersDeltas;
        layersDeltas.resize(network.size());
        std::vector<Matrix> layersDiff;
        layersDiff.resize(network.size());

        Matrix in,out;
        out=currentInput;
        for(int i=0;i<network.size();i++){
            in=out;
            out=network[i]*in;
            if(activationFunctions[i]){
                activationFunctions[i](out);
            }
            layersOutputs[i]=out;
        }


        //Calculate Deltas
        for(int i=network.size()-1,flag=0;i>=0;i--){
            if(!flag){
                flag++;
                layersDeltas[i]=(layersOutputs[i]-currentExpected);
                layersDeltas[i]*=(2.0f/(double)network[i].Height());
                continue;
            }
            layersDeltas[i]=network[i+1].Transpose()*layersDeltas[i+1];
        }

        //Use devFunctions
        for(int i=0;i<network.size();i++){
            if(devActivationFunctions[i]) {
                Matrix derv = layersOutputs[i];
                devActivationFunctions[i](derv);
                for (int j = 0; j < derv.Height(); j++) {
                    layersDeltas[i][j][0] *= derv[j][0];
                }
            }
        }
        //Calculate Weight diff

        for(int i=0,flag=0;i<network.size();i++){
            if(!flag){
                flag++;
                layersDiff[i]=layersDeltas[i]->*currentInput;
                continue;
            }
            layersDiff[i]=layersDeltas[i]->*layersOutputs[i-1];
        }

        //Update Weights

        for(int i=0;i<network.size();i++){
            network[i]-=layersDiff[i]*alpha;
        }

    }
}

void NeuralNetwork::SaveToFile(const char *fileName) {
    FILE *f= fopen(fileName,"wb");

    int temp=(int)network.size();
    fwrite(&temp,sizeof(int),1,f);
    for(int i=0;i<network.size();i++){
            for (int j = 0; j < DUMMY - 1; j++) {
                if(ActivationFunctions::GetFunc((FunctionsSelection)j)==activationFunctions[i]){
                    temp=j;
                    break;
                }
            }
        fwrite(&temp,sizeof(int),1,f);
    }
    for(int i=0;i<network.size();i++){
        for (int j = 0; j < DUMMY - 1; j++) {
            if(ActivationFunctions::GetFunc((FunctionsSelection)j)==devActivationFunctions[i]){
                temp=j;
                break;
            }
        }
        fwrite(&temp,sizeof(int),1,f);
    }

    for(int i=0;i<network.size();i++){
        temp=(int)network[i].Width();
        fwrite(&temp,sizeof(int),1,f);
        temp=(int)network[i].Height();
        fwrite(&temp,sizeof(int),1,f);

        for(int y=0;y<network[i].Height();y++){
            for(int x=0;x<network[i].Width();x++){
                double tempD=network[i][y][x];
                fwrite(&tempD,sizeof(double),1,f);
            }
        }

    }

    fclose(f);
}

void NeuralNetwork::LoadFromFile(const char *fileName) {
    FILE *f= fopen(fileName,"rb");
    if(!f)return;
    int temp;
    fread(&temp,sizeof(int),1,f);
    network.resize(temp);
    activationFunctions.resize(temp);
    devActivationFunctions.resize(temp);

    for(int i=0;i<network.size();i++){
        fread(&temp,sizeof(int),1,f);
        activationFunctions[i]=(void(*)(Matrix&))ActivationFunctions::GetFunc((FunctionsSelection)temp);
    }

    for(int i=0;i<network.size();i++){
        fread(&temp,sizeof(int),1,f);
        devActivationFunctions[i]=(void(*)(Matrix&))ActivationFunctions::GetFunc((FunctionsSelection)temp);
    }

    for(int i=0;i<network.size();i++){
        int h,w;
        fread(&w,sizeof(int),1,f);
        fread(&h,sizeof(int),1,f);
        Matrix loadMatrix(w,h);

        double tempD;
        for(int y=0;y<loadMatrix.Height();y++){
            for(int x=0;x<loadMatrix.Width();x++){
                fread(&tempD,sizeof(double),1,f);
                loadMatrix[y][x]=tempD;
            }
        }
        network[i]=loadMatrix;
    }

    fclose(f);
}