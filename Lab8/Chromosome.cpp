#include "Chromosome.h"
#include "MyRandom.h"

Chromosome::Chromosome(int length) {

    genes.resize(length);
    this->length=length;
    for(int i=0;i<length;i++){
        genes[i]=Random(1);
    }
}

void Chromosome::Mutate(float chance,bool full) {
    int chancePer=chance*100;

    if(full){
        for(int i=0;i<length;i++){
            if (chancePer >= Random(100)) {
                if (genes[i]) {
                    genes[i] = 0;
                } else {
                    genes[i] = 1;
                }
            }
        }
    }
    else {
        if (chancePer >= Random(100)) {
            int pos = Random(length - 1);
            if (genes[pos]) {
                genes[pos] = 0;
            } else {
                genes[pos] = 1;
            }
        }
    }
}

int &Chromosome::operator[](int index) {
    return genes[index];
}

Chromosome Chromosome::operator+(Chromosome b) {
    Chromosome result(length);
    int i=0;
    for(;i<length/2;i++){
        result[i]=genes[i];
    }
    for(;i<length;i++){
        result[i]=b[i];
    }

    return result;
}

float Chromosome::evaluate(float (*func)(std::vector<int> &)) {
    return func(genes);
}