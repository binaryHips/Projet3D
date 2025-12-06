#include <iostream>

#include "includes_all_cpu.h"

// contains tests for the core features

int main(int, char**){
    GeoContextCPU context = GeoContextCPU::createGeoContext();

    context.maps[to_underlying(MAP_LAYERS::BEDROCK)](10, 10) = 0.2;
    context.maps[to_underlying(MAP_LAYERS::STONE)](10, 10) = 0.0;
    context.maps[to_underlying(MAP_LAYERS::SAND)](10, 10) = 0.0;
    context.maps[to_underlying(MAP_LAYERS::WATER)](10, 10) = 0.0;

    u8 idx = -1;
    std::cout << context.totalHeight(uvec2(10, 10), &idx) << std::endl;
    std::cout << (u32)idx << std::endl;
}