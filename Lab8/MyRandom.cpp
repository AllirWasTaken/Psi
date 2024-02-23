#include "MyRandom.h"
#include <ctime>
#include <cstdlib>



void __attribute__((constructor))init(){
    srand(time(nullptr));
}



int Random(int max) {
    auto val =rand();
    return (val)%(max+1);
}