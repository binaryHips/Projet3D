cmake_minimum_required(VERSION 3.10.0)


project(Projet3D VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(core)

add_executable(app
	main.cpp
)

target_link_libraries(app core)
