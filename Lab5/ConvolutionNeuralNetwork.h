#ifndef LAB5_CONVOLUTIONNEURALNETWORK_H
#define LAB5_CONVOLUTIONNEURALNETWORK_H

#include "Matrix.h"

ALib::Matrix ConvolutionTask1(ALib::Matrix &input,ALib::Matrix &filtr, int stride);
ALib::Matrix ConvolutionRun2(const ALib::Matrix &input,const ALib::Matrix& convolution, ALib::Matrix& neuralGrid);
ALib::Matrix ConvolutionRun3(const ALib::Matrix &input);
ALib::Matrix ConvolutionUpdate2(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid, float alpha);
ALib::Matrix ConvolutionUpdate3(ALib::Matrix &input, ALib::Matrix &expected);
int Test2(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid);
int Test3(ALib::Matrix &input, ALib::Matrix &expected);





#endif //LAB5_CONVOLUTIONNEURALNETWORK_H
