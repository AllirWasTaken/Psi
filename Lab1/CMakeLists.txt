cmake_minimum_required(VERSION 3.26)
project(Lab1)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab1 main.cpp NeuronLayer.cpp Neuron.cpp Neuron.hpp NeuronLayer.h)
