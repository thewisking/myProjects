cmake_minimum_required(VERSION 3.16)
project(NonogramSolver VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nonogram_solver
    src/main.cpp
)
