cmake_minimum_required(VERSION 3.26)
project(Lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab3 main.cpp
        ActivationFunctions.cpp ActivationFunctions.h
        Matrix.h Matrix.cpp
        NeuralNetwork.cpp NeuralNetwork.h
)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")