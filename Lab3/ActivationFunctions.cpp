//
// Created by Allir on 23.10.2023.
//

#include "ActivationFunctions.h"


double ActivationFunctions::RectifiedLinearUnit(double x) {
    if(x<=0)return 0;
    else return x;
}

double ActivationFunctions::RectifiedLinearUnitDer(double x) {
    if(x<=0)return 0;
    else return x;
}


double ActivationFunctions::SigmaFunc(double x) {
    return x*(1-x);
}