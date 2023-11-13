#include <thread>
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
            newMatrix[y][x]=(float)rand()/(float)RAND_MAX*20-10;
        }
    }

    network.push_back(newMatrix);
    activationFunctions.push_back(aFunc);
    devActivationFunctions.push_back(aFuncDev);
}

void NeuralNetwork::Update(const ALib::Matrix &input, const ALib::Matrix &expected, float alpha, float dropOut) {
    //initDropOut
    std::vector<Matrix> dropOutMatrix;
    dropOutMatrix.resize(network.size()-1);
    if(dropOut!=0){
        Matrix in,out(1,input.Height());
        for (int i = 0; i < network.size()-1; i++) {
            in = out;
            out = network[i] * in;
            dropOutMatrix[i]=NeuralNetwork::GetDropOutMatrix(out,dropOut);
        }
    }
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



        //Use DropOut
        if(dropOut!=0) {
            for (int i = 0; i < network.size() - 1; i++) {
                layersOutputs[i] = layersOutputs[i].MultiplyIndexByIndex(dropOutMatrix[i]);
            }
        }

        //Calculate Deltas
        for(int i=network.size()-1,flag=0;i>=0;i--){
            if(!flag){
                flag++;
                layersDeltas[i]=(layersOutputs[i]-currentExpected);
                layersDeltas[i]*=(2.0f/(float)network[i].Height());
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
        //use DropOut on delta
        if(dropOut!=0) {
            for (int i = 0; i < network.size() - 1; i++) {
                layersDeltas[i] = layersDeltas[i].MultiplyIndexByIndex(dropOutMatrix[i]);
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
                float tempD=network[i][y][x];
                fwrite(&tempD,sizeof(float),1,f);
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

        float tempD;
        for(int y=0;y<loadMatrix.Height();y++){
            for(int x=0;x<loadMatrix.Width();x++){
                fread(&tempD,sizeof(float),1,f);
                loadMatrix[y][x]=tempD;
            }
        }
        network[i]=loadMatrix;
    }

    fclose(f);
}

unsigned int NeuralNetwork::TestAndScore(const ALib::Matrix &input, const ALib::Matrix &expected) {
    unsigned result=0;

    Matrix answers;
    answers=Run(input);



    for(int i=0;i<expected.Width();i++){
        unsigned correct=11;
        for(int j=0;j<expected.Height();j++){
            if(expected[j][i]==1){
                correct=j;
                break;
            }
        }
        float max=answers[0][i];
        unsigned maxI=0;
        for(int j=0;j<expected.Height();j++){
            if(answers[j][i]>max){
                maxI=j;
                max=answers[j][i];
            }
        }
        if(maxI==correct)result++;
    };



    return result;
}

ALib::Matrix NeuralNetwork::GetDropOutMatrix(ALib::Matrix &matrixFor, float dropOut) {
    Matrix dropOutMatrix(matrixFor.Width(),matrixFor.Height());
    unsigned droppedNeurons=matrixFor.Height();
    droppedNeurons= droppedNeurons * dropOut;
    float droppedNeuronsOpposite;
    droppedNeuronsOpposite=1/(1.0f-(float)droppedNeurons/(float)matrixFor.Height());
    dropOutMatrix.SetZero();

    int chance;
    if(dropOut>=0.5f||dropOut==0){
        chance=2;
    }
    else{
        chance=(int)((float)1/(float)dropOut);
    }

    srand(time(nullptr));
    for(int neuronsToDrop=droppedNeurons,pos=0;neuronsToDrop;pos++){
        if(pos==matrixFor.Height())pos=0;
        if(rand()%chance==0&&dropOutMatrix[pos][0]!=-1){
            neuronsToDrop--;
            dropOutMatrix[pos][0]=-1;
        }
    }
    for(int i=0;i<dropOutMatrix.Height();i++){
        if(dropOutMatrix[i][0]==-1){
            dropOutMatrix[i][0]=0;
        }
        else{
            for(int j=0;j<dropOutMatrix.Width();j++){
                dropOutMatrix[i][j]=droppedNeuronsOpposite;
            }
        }
    }
    return dropOutMatrix;
}

void NeuralNetwork::UpdateMiniBatch(const ALib::Matrix &input, const ALib::Matrix &expected, float alpha,
                                    unsigned int batchSize, unsigned iterationsCount, float dropOut) {
    unsigned threadCount=input.Width()/batchSize;
    if(input.Width()%batchSize!=0)threadCount++;
    std::thread **threads;
    std::vector<Matrix> inputThreads;
    std::vector<Matrix> expectedThreads;
    std::vector<std::vector<Matrix>> outputThreads;
    std::vector<BatchedThreadingArgs> args;
    inputThreads.resize(threadCount);
    expectedThreads.resize(threadCount);
    outputThreads.resize(threadCount);
    args.resize(threadCount);
    threads = new std::thread *[threadCount];

    //divide inputs between threads
    for(int i=0;i<threadCount;i++){
        unsigned currentSize=batchSize;
        if(i + 1 == threadCount){
            currentSize=input.Width()%batchSize;
            if(currentSize==0)currentSize=batchSize;
        }
        Matrix newInput(currentSize,input.Height());
        Matrix newExpected(currentSize,expected.Height());

        for(int y=0;y<input.Height();y++){
            for(int x=0;x<currentSize;x++){
                newInput[y][x]=input[y][x+i*batchSize];
            }
        }

        for(int y=0;y<expected.Height();y++){
            for(int x=0;x<currentSize;x++){
                newExpected[y][x]=expected[y][x+i*batchSize];
            }
        }


        inputThreads[i]=newInput;
        expectedThreads[i]=newExpected;
    }

    //initDropOut
    std::vector<Matrix> dropOutMatrix;
    dropOutMatrix.resize(network.size()-1);
    if(dropOut!=0){
        Matrix in,out(batchSize,input.Height());
        for (int i = 0; i < network.size()-1; i++) {
            in = out;
            out = network[i] * in;
            dropOutMatrix[i]=NeuralNetwork::GetDropOutMatrix(out,dropOut);
        }
    }

    //Launch batching on multiple thread
    for(int i=0;i<threadCount;i++){
        args[i].alpha=alpha;
        args[i].input=&inputThreads[i];
        args[i].expected=&expectedThreads[i];
        args[i].network= *this;
        args[i].output= &outputThreads[i];
        args[i].dropOutMatrix=&dropOutMatrix;
        args[i].dropOut=dropOut;
    }

    for(int iteration=0;iteration<iterationsCount;iteration++) {
        for (int i = 0; i < threadCount; i++) {
            threads[i] = new std::thread(BatchedUpdateWeightDiff, args.data() + i);
        }
        for (int i = 0; i < threadCount; i++) {
            threads[i]->join();
        }
        
        
        //collect results and update network
        std::vector<Matrix> finalWeightDiff;
        finalWeightDiff.resize(network.size());
        for (int i = 0; i < network.size(); i++) {
            finalWeightDiff[i].SetZero();
            Matrix newWeight(outputThreads[0][i].Width(),outputThreads[0][i].Height());
            finalWeightDiff[i]=newWeight;
            for (int j = 0; j < threadCount; j++) {
                finalWeightDiff[i] += outputThreads[j][i];
            }
            finalWeightDiff[i] /= (float) threadCount;
        }

        for (int i = 0; i < network.size(); i++) {
            network[i] -= finalWeightDiff[i];
        }


        for (int i = 0; i < threadCount; i++) {
            delete threads[i];
        }
    }
    delete []threads;
}

void NeuralNetwork::BatchedUpdateWeightDiff(BatchedThreadingArgs *args) {
    NeuralNetwork &tNetwork=args->network;
    

    std::vector<Matrix> layersOutputs;
    layersOutputs.resize(tNetwork.network.size());
    std::vector<Matrix> layersDeltas;
    layersDeltas.resize(tNetwork.network.size());
    std::vector<Matrix> layersDiff;
    layersDiff.resize(tNetwork.network.size());


    Matrix in,out;
    out=*args->input;
    for(int i=0;i<tNetwork.network.size();i++){
        in=out;
        out=tNetwork.network[i]*in;
        if(tNetwork.activationFunctions[i]){
            tNetwork.activationFunctions[i](out);
        }
        layersOutputs[i]=out;
    }



    //Use DropOut
    if(args->dropOut!=0) {
        for (int i = 0; i < tNetwork.network.size() - 1; i++) {
            layersOutputs[i] = layersOutputs[i].MultiplyIndexByIndex((*args->dropOutMatrix)[i]);
        }
    }

    //Calculate Deltas
    for(int i=tNetwork.network.size()-1,flag=0;i>=0;i--){
        if(!flag){
            flag++;
            layersDeltas[i]=(layersOutputs[i]-(*args->expected));
            layersDeltas[i]*=(2.0f/(float)tNetwork.network[i].Height());
            layersDeltas[i]/=(float)args->input->Width();
            continue;
        }
        layersDeltas[i]=tNetwork.network[i+1].Transpose()*layersDeltas[i+1];
    }

    //Use devFunctions
    for(int i=0;i<tNetwork.network.size();i++){
        if(tNetwork.activationFunctions[i]) {
            Matrix derv = layersOutputs[i];
            tNetwork.activationFunctions[i](derv);
            layersDeltas[i]=layersDeltas[i].MultiplyIndexByIndex(derv);
        }
    }
    //use DropOut on delta
    if(args->dropOut!=0) {
        for (int i = 0; i < tNetwork.network.size() - 1; i++) {
            layersDeltas[i] = layersDeltas[i].MultiplyIndexByIndex((*args->dropOutMatrix)[i]);
        }
    }

    //Calculate Weight diff

    for(int i=0,flag=0;i<tNetwork.network.size();i++) {
        if (!flag) {
            flag++;
            layersDiff[i] = layersDeltas[i]*args->input->Transpose();
            continue;
        }
        layersDiff[i] = layersDeltas[i]*layersOutputs[i - 1].Transpose();
    }
        //Update Weights
    *(args->output)=layersDiff;
    return;
}