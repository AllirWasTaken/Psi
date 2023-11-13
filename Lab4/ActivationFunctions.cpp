//
// Created by Allir on 23.10.2023.
//

#include "ActivationFunctions.h"


void ActivationFunctions::RectifiedLinearUnit(ALib::Matrix & matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
            if(matrix[y][x]<=0) {
                matrix[y][x] = 0;
            }
        }
    }
}

void ActivationFunctions::RectifiedLinearUnitDer(ALib::Matrix & matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
            if(matrix[y][x]>0) {
                matrix[y][x] =1;
            }
            else{
                matrix[y][x] =0;
            }
        }
    }
}


double ActivationFunctions::SigmaFunc(double x) {
    return x*(1-x);
}

void *ActivationFunctions::GetFunc(FunctionsSelection id) {
    switch (id) {
        case RLU: return (void*)ActivationFunctions::RectifiedLinearUnit;
        case D_RLU: return (void*)ActivationFunctions::RectifiedLinearUnitDer;
        default: return nullptr;
    }
}