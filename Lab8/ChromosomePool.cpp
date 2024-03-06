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
        if(i==70){
            int a=5;
        }
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

void ChromosomePool::Rulette2(float fraction,float mutation,bool full) {
    std::vector<float> score;
    score.resize(size);

    float scoreSum=0;

    for(int i=0;i<size;i++){
        score[i]=population[i].evaluate(evaluationFunction);
        scoreSum+=score[i];
    }
    std::vector<float> scoreCul;
    scoreCul.resize(size);
    scoreCul[0]=score[0];
    for(int i=1;i<size;i++){
        scoreCul[i]=scoreCul[i-1]+score[i];
    }
    std::vector<float> chance;
    chance.resize(size);

    for(int i=0;i<size;i++){
        chance[i]=scoreCul[i]/scoreSum;
    }

    int winnerSize=(float)size*fraction;
    std::vector<Chromosome> winners;
    for(int i=0;i<winnerSize;i++){
        float chosen= (float)Random(100)/(float)100;
        int chosenIndex=0;
        for(int j=0;j<size;j++){
            if(chance[j]>=chosen){
                chosenIndex=j;
                break;
            }
        }
        winners.push_back(population[chosenIndex]);

    }

    std::vector<Chromosome> newPopulation;

    for(int i=0;i<winnerSize-1;i++){
        newPopulation.push_back(winners[i]+winners[i+1]);
        newPopulation.push_back(winners[i+1]+winners[i]);
        if(newPopulation.size()==size)break;
    }
    if(newPopulation.size()+2==size) {
        newPopulation.push_back(winners[0] + winners[winners.size() - 1]);
        newPopulation.push_back(winners[winners.size() - 1] + winners[0]);
    }

    for(int i=0;i<size;i++){
        newPopulation[i].Mutate(mutation, full);
    }


    population=newPopulation;

}

void ChromosomePool::Rulette3(float mutation) {
    auto elita = GetBest(0.25);
    Rulette2(1,mutation, true);

    for(int i=0;i<elita.size();i++){
        population[i]=elita[i];
    }

}