cmake_minimum_required(VERSION 3.10.0)
project(core VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-mavx2 -mavx512f)
add_library(core STATIC
    defines.h

    maths/maths.h

    commons/allocator.h

    commons/map/map.h

    commons/particle_system/particle_system.h
    commons/geo_context/geo_context.h

    cpu/geo_context/geo_context.h
    cpu/geo_context/processes.cpp

    cpu/particle_system/particle_system.h
    cpu/particle_system/particle_system.cpp


    cpu/map/map.h
    cpu/map/map.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
