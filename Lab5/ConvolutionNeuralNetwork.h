#ifndef LAB5_CONVOLUTIONNEURALNETWORK_H
#define LAB5_CONVOLUTIONNEURALNETWORK_H

#include "Matrix.h"

ALib::Matrix ConvolutionTask1(ALib::Matrix &input,ALib::Matrix &filtr, int stride);
ALib::Matrix ConvolutionRun2(const ALib::Matrix &input,const ALib::Matrix& convolution, ALib::Matrix& neuralGrid);
ALib::Matrix ConvolutionRun3(const ALib::Matrix &input,const ALib::Matrix& convolution, ALib::Matrix& neuralGrid);
void ConvolutionUpdate2(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid, float alpha);
void ConvolutionUpdate3(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid, float alpha);
int Test2(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid);
int Test3(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid);





#endif //LAB5_CONVOLUTIONNEURALNETWORK_H
