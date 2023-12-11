#ifndef LAB2_ACTIVATIONFUNCTIONS_H
#define LAB2_ACTIVATIONFUNCTIONS_H
#include "Matrix.h"

enum FunctionsSelection{
    NONE,
    RLU,
    D_RLU,
    SIG,
    TANH,
    SOFT,
    D_SIG,
    D_TANH,
    DUMMY
};

class ActivationFunctions {
public:
    static void* GetFunc(FunctionsSelection id);
    static void RectifiedLinearUnit(ALib::Matrix &);
    static void RectifiedLinearUnitDer(ALib::Matrix &);
    static void Sigmoidal(ALib::Matrix &);
    static void SigmoidalDer(ALib::Matrix &);
    static void TangensHiperbolic(ALib::Matrix &);
    static void TangensHiperbolicDer(ALib::Matrix &);
    static void SoftMax(ALib::Matrix &);
};


#endif //LAB2_ACTIVATIONFUNCTIONS_H
