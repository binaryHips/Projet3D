cmake_minimum_required(VERSION 3.10.0)
project(core VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
	includes
    commons
    .
)

add_library(core
    defines.h

    commons/allocator.h

    commons/image/image.h
    commons/image/image.cpp

    commons/particleSystem/particleSystem.h
    commons/particleSystem/particleSystem.cpp
)
