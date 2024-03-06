cmake_minimum_required(VERSION 3.26)
project(Lab8)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab8 main.cpp
        Chromosome.cpp
        Chromosome.h
        ChromosomePool.cpp
        ChromosomePool.h
        MyRandom.cpp
        MyRandom.h)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")