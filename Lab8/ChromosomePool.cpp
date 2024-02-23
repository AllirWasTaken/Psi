#include "ChromosomePool.h"
#include "MyRandom.h"
ChromosomePool::ChromosomePool(int size,int length,bool ran) {
    population.resize(size);
    for(int i=0;i<size;i++){
        population[i].genes.resize(length);
        population[i].length=length;
        for(int j=0;j<length;j++){
            if(ran)population[i][j]= Random(1);
            else population[i][j]=0;
        }

    }
    evaluationFunction=nullptr;
    this->size=size;
}


void ChromosomePool::SortPops(std::vector<float> &score) {
    int work=1;
    while(work){
        work=0;
        for(int j=0;j<score.size()-1;j++){
            if(score[j]<score[j+1]){
                work=1;
                auto temp=score[j+1];
                score[j+1]=score[j];
                score[j]=temp;
                auto tempPop=population[j+1];
                population[j+1]=population[j];
                population[j]=tempPop;
            }
        }
    }

}

std::vector<Chromosome> ChromosomePool::GetBest(float fraction) {
    std::vector<float> score;
    score.resize(size);

    for(int i=0;i<size;i++){
        score[i]=population[i].evaluate(evaluationFunction);
    }


    SortPops(score);

    int bestSize=size*fraction;
    std::vector<Chromosome> result;
    for(int i=0;i<bestSize;i++){
        result.push_back(population[i]);
    }

    return result;
}

Chromosome &ChromosomePool::operator[](int index) {
    return population[index];
}