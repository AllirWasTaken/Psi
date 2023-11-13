cmake_minimum_required(VERSION 3.26)
project(Lab4)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab4 main.cpp
        ActivationFunctions.cpp ActivationFunctions.h
        Matrix.h Matrix.cpp
        NeuralNetwork.cpp NeuralNetwork.h
)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")