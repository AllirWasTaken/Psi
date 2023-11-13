#ifndef LAB2_ACTIVATIONFUNCTIONS_H
#define LAB2_ACTIVATIONFUNCTIONS_H
#include "Matrix.h"

enum FunctionsSelection{
    NONE,
    RLU,
    D_RLU,
    DUMMY
};

class ActivationFunctions {
public:
    static void* GetFunc(FunctionsSelection id);
    static void RectifiedLinearUnit(ALib::Matrix &);
    static void RectifiedLinearUnitDer(ALib::Matrix &);
    static double SigmaFunc(double);

};


#endif //LAB2_ACTIVATIONFUNCTIONS_H
