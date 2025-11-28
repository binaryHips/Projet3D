#include <iostream>

#include "includes_all_cpu.h"

int main(int, char**){
    GeoContextCPU context = GeoContextCPU::createGeoContext();
    context.update(0.1);
    context.update(0.1);
    context.update(0.1);
    context.update(0.1);
    context.update(0.1);
    context.update(0.1);
    context.update(0.1);
}