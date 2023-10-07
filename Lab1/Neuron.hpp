



namespace PSI {

    class Neuron {
    public:
        Neuron(int inputCount,float bias);
        ~Neuron();
        float NeuronRun(float *inputs);
        int GetSize();
        void Randomize();



    private:
        float bias;
        int size;
        float *weights;
    };


}