#include "ActivationFunctions.h"
#include <cmath>


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

void ActivationFunctions::Sigmoidal(ALib::Matrix &matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
            matrix[y][x]=1.0f/(1+(float)exp(-matrix[y][x]));
        }
    }
}

void ActivationFunctions::SigmoidalDer(ALib::Matrix &matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
           matrix[y][x]=matrix[y][x]*(1-matrix[y][x]);
        }
    }
}

void ActivationFunctions::TangensHiperbolic(ALib::Matrix &matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
            matrix[y][x]=(float)((exp(matrix[y][x])-exp(-matrix[y][x]))/(exp(matrix[y][x])+exp(-matrix[y][x])));
        }
    }
}
void ActivationFunctions::TangensHiperbolicDer(ALib::Matrix &matrix) {
    for(int y=0;y<matrix.Height();y++){
        for(int x=0;x<matrix.Width();x++){
            matrix[y][x]=1-(matrix[y][x]*matrix[y][x]);
        }
    }
}
void ActivationFunctions::SoftMax(ALib::Matrix &matrix) {
    for(int x=0;x<matrix.Width();x++){
        float sum=0;
        for(int y=0;y<matrix.Height();y++){
            sum+=(float)exp(matrix[y][x]);
        }
        for(int y=0;y<matrix.Height();y++){
            matrix[y][x]=(float)exp(matrix[y][x])/sum;
        }
    }
}

void *ActivationFunctions::GetFunc(FunctionsSelection id) {
    switch (id) {
        case RLU: return (void*)ActivationFunctions::RectifiedLinearUnit;
        case D_RLU: return (void*)ActivationFunctions::RectifiedLinearUnitDer;
        case SIG: return (void*)ActivationFunctions::Sigmoidal;
        case TANH: return (void*)ActivationFunctions::TangensHiperbolic;
        case SOFT: return (void*)ActivationFunctions::SoftMax;
        case D_SIG: return (void*)ActivationFunctions::SigmoidalDer;
        case D_TANH: return (void*)ActivationFunctions::TangensHiperbolicDer;
        default: return nullptr;
    }
}