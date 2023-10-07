#include "Neuron.hpp"
#include <cstdlib>
#include <ctime>

PSI::Neuron::Neuron(int inputCount,float bias){
    weights=new float[inputCount];
    size=inputCount;
    this->bias=bias;
}

PSI::Neuron::~Neuron(){
    delete weights;
}

float PSI::Neuron::NeuronRun(float *inputs) {

    float sum=0;
    for(int i=0;i<size;i++){
        sum+=inputs[i]*weights[i];
    }
    sum+=bias;

    return sum;
}

void PSI::Neuron::Randomize(){
    srand(time(NULL));
    for(int i=0;i<size;i++){
        weights[i]=(float)(rand()%201-100)/100;
    }
}
