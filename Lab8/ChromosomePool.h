#ifndef LAB8_CHROMOSOMEPOOL_H
#define LAB8_CHROMOSOMEPOOL_H
#include "Chromosome.h"

class ChromosomePool {
public:
    std::vector<Chromosome> population;
    int size;
    float (*evaluationFunction)(std::vector<int>&);
    ChromosomePool(int size,int length,bool ran=true);
    Chromosome& operator[](int index);
    std::vector<Chromosome> GetBest(float fraction);
private:
     void SortPops(std::vector<float> &score);

};


#endif //LAB8_CHROMOSOMEPOOL_H
