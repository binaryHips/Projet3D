#include <iostream>

#include "includes_all_cpu.h"

int main(int, char**){
    GeoContextCPU context;

    context.addMap(MapCPU(0)); // rock
    context.addMap(MapCPU(1)); // sand
    context.addMap(MapCPU(1)); // water
}