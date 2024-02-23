#include <iostream>
#include <vector>
#include "ChromosomePool.h"


float EvaluationZad1(std::vector<int>& vector){
    int sum=0;
    for (int i = 0; i < vector.size(); i++)
        sum += vector[i];

    return (float)sum;
}


int main() {
    //Zad1
    {
        std::cout<<"Zad1\n\n";
        ChromosomePool pool(10, 10,false);
        pool.evaluationFunction = EvaluationZad1;
        int era=0;
        float target=10;
        float last=0;
        while(true) {
            auto best = pool.GetBest(0.2f);
            float current=best[0].evaluate(EvaluationZad1);
            if(last!=current) {
                last= current;
                std::cout << "Era: " << era << "\nEvaluation: " << current << "/" << target<<std::endl;
            }
            if(target==current)break;

            Chromosome new1 = best[0] + best[1];
            Chromosome new2 = best[1] + best[0];

            new1.Mutate(0.6);
            new2.Mutate(0.6);

            pool[8] = new1;
            pool[9] = new2;
            era++;
        }
    }

    //Zad2
    {
        std::cout<<"Zad2\n\n";
        ChromosomePool pool(10, 10,false);
        pool.evaluationFunction = EvaluationZad1;
        int era=0;
        float target=10;
        float last=0;
        while(true) {
            auto best = pool.GetBest(0.2f);
            float current=best[0].evaluate(EvaluationZad1);
            if(last!=current) {
                last= current;
                std::cout << "Era: " << era << "\nEvaluation: " << current << "/" << target<<std::endl;
            }
            if(target==current)break;

            Chromosome new1 = best[0] + best[1];
            Chromosome new2 = best[1] + best[0];

            new1.Mutate(0.6);
            new2.Mutate(0.6);

            pool[8] = new1;
            pool[9] = new2;
            era++;
        }
    }




    return 0;
}
