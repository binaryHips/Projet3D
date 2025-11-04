cmake_minimum_required(VERSION 3.10.0)
project(Core VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(Core
    defines.h

    commons/allocator.h

    commons/image/image.h
    commons/image/image.cpp

    commons/particleSystem/particleSystem.h
    commons/particleSystem/particleSystem.cpp
)

