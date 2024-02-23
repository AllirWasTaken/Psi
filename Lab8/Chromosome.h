#ifndef LAB8_CHROMOSOME_H
#define LAB8_CHROMOSOME_H
#include <vector>

class Chromosome {
public:
    Chromosome(int length=0);
    std::vector<int> genes;
    void Mutate(float chance);
    Chromosome operator+(Chromosome b);
    int length;
    int& operator[](int index);
    float evaluate(float(*func)(std::vector<int>&));


};


#endif //LAB8_CHROMOSOME_H
