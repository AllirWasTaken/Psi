cmake_minimum_required(VERSION 3.26)
project(Lab5)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab5 main.cpp
        ConvolutionNeuralNetwork.cpp
        ConvolutionNeuralNetwork.h
        Matrix.cpp Matrix.h)


set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")