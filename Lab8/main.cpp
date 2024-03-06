#include <iostream>
#include <vector>
#include "ChromosomePool.h"
#include <cmath>


float EvaluationZad1(std::vector<int>& vector){
    int sum=0;
    for (int i = 0; i < vector.size(); i++)
        sum += vector[i];

    return (float)sum;
}

float EvaluationZad2(std::vector<int>& vector){
    int a=0,b=0;
    a+=8*vector[0];
    a+=4*vector[1];
    a+=2*vector[2];
    a+=1*vector[3];

    b+=8*vector[4];
    b+=4*vector[5];
    b+=2*vector[6];
    b+=1*vector[7];

    int result = 2*a*a+b;
    int diffrence=33-result;
    if(diffrence<0)diffrence*=-1;

    int score=200-diffrence;
    if(score<0) score=0;

    return score;
}



struct backpackElement{
    int weight,value;
};

std::vector<backpackElement> backpack;
float EvaluationZad3(std::vector<int>& vector){
    float weight=0,value=0;

    for(int i=0;i<vector.size();i++){
        if(vector[i]) {
            weight += backpack[i].weight;
            value+= backpack[i].value;
        }
    }
    if(weight<=35)return value;
    else return 0;
}

class city{
public:
    int x, y;
    float distance(city& b){
        return sqrt(pow(x-b.x,2)+pow(y-b.y,2));
    }
};

std::vector<city> cities;
float EvaluationZad4(std::vector<int>& vector){
    std::vector<int> path;
    path.resize(24);

    for(int i=0;i<24;i++){
        int current=0;
        current+=16*vector[i*5+0];
        current+=8*vector[i*5+1];
        current+=4*vector[i*5+2];
        current+=2*vector[i*5+3];
        current+=1*vector[i*5+4];
        path[i]=current;
    }
    std::vector<int> check;
    check.resize(40);

    for(int i=0;i<24;i++){
        check[path[i]]++;
    }
    int lacking=0;
    for(int i=0;i<24;i++){
        if(check[i]!=1)lacking++;
    }

    float totalDistance=0;
    city currentCity=cities[path[0]];
    for(int i=1;i<24;i++){
        city nextCity=cities[path[i]];
        totalDistance+=currentCity.distance(nextCity);
        currentCity=nextCity;
    }

    float score=10000-totalDistance-500*lacking;

    if(score<0)score=0;

    return score;

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
        ChromosomePool pool(10, 8, false);
        pool.evaluationFunction = EvaluationZad2;
        int era=0;
        float target=200;
        float last=-1;
        while(true) {
            auto best = pool.GetBest(0.2f);
            float current = best[0].evaluate(EvaluationZad2);
            if (last != current) {
                last = current;
                std::cout << "Era: " << era << "\nEvaluation: " << current << "/" << target << std::endl;
            }

            int a = 0, b = 0;

            a += 8 * best[0][0];
            a += 4 * best[0][1];
            a += 2 * best[0][2];
            a += 1 * best[0][3];

            b += 8 * best[0][4];
            b += 4 * best[0][5];
            b += 2 * best[0][6];
            b += 1 * best[0][7];
            if (target == current) {
                std::cout << "a: " << a << "b: " << b << std::endl;
                break;
            }

            pool.Rulette2(0.5,0.1);
            era++;
        }
    }
    //Zad3
    {
        backpack.push_back(backpackElement{
            .weight=3,
            .value=266
        });
        backpack.push_back(backpackElement{
                .weight=13,
                .value=442
        });
        backpack.push_back(backpackElement{
                .weight=10,
                .value=671
        });
        backpack.push_back(backpackElement{
                .weight=9,
                .value=526
        });
        backpack.push_back(backpackElement{
                .weight=7,
                .value=388
        });
        backpack.push_back(backpackElement{
                .weight=1,
                .value=245
        });
        backpack.push_back(backpackElement{
                .weight=8,
                .value=210
        });
        backpack.push_back(backpackElement{
                .weight=8,
                .value=145
        });
        backpack.push_back(backpackElement{
                .weight=2,
                .value=126
        });
        backpack.push_back(backpackElement{
                .weight=9,
                .value=322
        });

        std::cout<<"Zad3\n\n";
        ChromosomePool pool(8, 10, false);
        pool.evaluationFunction = EvaluationZad3;
        int era=0;
        float target=2222;
        float last=-1;
        while(true) {
            auto best = pool.GetBest(0.2f);
            float current = best[0].evaluate(EvaluationZad3);
            if (last != current) {
                last = current;
                std::cout << "Era: " << era << "\nEvaluation: " << current << "/" << target << std::endl;
            }


            if (target == current) {
                break;
            }

            pool.Rulette3(0.05);
            era++;
        }


    }
    //Zad 4
    {
        cities.push_back(city{.x=119,.y=38});//0
        cities.push_back(city{.x=37,.y=38});//1
        cities.push_back(city{.x=197,.y=55});//2
        cities.push_back(city{.x=85,.y=165});//3
        cities.push_back(city{.x=12,.y=50});//4
        cities.push_back(city{.x=100,.y=53});//5
        cities.push_back(city{.x=81,.y=142});//6
        cities.push_back(city{.x=121,.y=137});//7
        cities.push_back(city{.x=85,.y=145});//8
        cities.push_back(city{.x=80,.y=197});//9
        cities.push_back(city{.x=91,.y=176});//10
        cities.push_back(city{.x=106,.y=55});//11
        cities.push_back(city{.x=123,.y=57});//12
        cities.push_back(city{.x=40,.y=81});//13
        cities.push_back(city{.x=78,.y=125});//14
        cities.push_back(city{.x=190,.y=46});//15
        cities.push_back(city{.x=187,.y=40});//16
        cities.push_back(city{.x=37,.y=107});//17
        cities.push_back(city{.x=17,.y=11});//18
        cities.push_back(city{.x=67,.y=56});//19
        cities.push_back(city{.x=78,.y=133});//20
        cities.push_back(city{.x=87,.y=23});//21
        cities.push_back(city{.x=184,.y=197});//22
        cities.push_back(city{.x=111,.y=12});//23
        cities.push_back(city{.x=66,.y=178});//24




        std::cout<<"Zad4\n\n";
        ChromosomePool pool(100, 120);
        pool.evaluationFunction = EvaluationZad4;
        int era=0;
        float target=9000;
        float last=-1;
        while(true) {
            auto best = pool.GetBest(0.2f);
            float current = best[0].evaluate(EvaluationZad4);
            if (last != current) {
                last = current;
                std::cout << "Era: " << era << "\nEvaluation: " << current << "/" << target << std::endl;
            }




            if (target <= current) {
                std::vector<int> path;
                path.resize(24);

                for(int i=0;i<24;i++){
                    int current=0;
                    current+=16*best[0][i*5+0];
                    current+=8*best[0][i*5+1];
                    current+=4*best[0][i*5+2];
                    current+=2*best[0][i*5+3];
                    current+=1*best[0][i*5+4];
                    path[i]=current;
                }
                float totalDistance=0;
                city currentCity=cities[path[0]];
                for(int i=1;i<24;i++){
                    city nextCity=cities[path[i]];
                    totalDistance+=currentCity.distance(nextCity);
                    currentCity=nextCity;
                }

                std::cout<<"Distance :"<<totalDistance<<"\nPath :";
                for(int i=0;i<24;i++){
                    std::cout<<path[i]<<" ";
                }
                break;
            }

            pool.Rulette3(0.01);
            era++;
        }


    }





    return 0;
}
