cmake_minimum_required(VERSION 3.10.0)
project(core VERSION 0.1.0 LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SAUVEUR https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
add_library(core SHARED
    defines.h

    commons/maths/maths.h

    commons/allocator.h

    commons/map/map.h
    commons/map/map.cpp

    commons/geo_context/geo_context.h

    cpu/geo_context/geo_context.h

    cpu/particle_system/particle_system.h

    cpu/map/map.h

    commons/particle_system/particleSystem.h
)

target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(core PROPERTIES PUBLIC_HEADER includes/includes_all_cpu.h)

set_property(TARGET core PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             ${PROJECT_SOURCE_DIR}/includes)


# Install library
install(TARGETS core DESTINATION lib/core)

install(FILES includes/*.h DESTINATION include/core)
