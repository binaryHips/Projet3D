cmake_minimum_required(VERSION 3.10.0)
project(Projet3D VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(core)

include_directories(
	core/includes
)

add_executable(app
	main.cpp
)

target_link_libraries(app core)
