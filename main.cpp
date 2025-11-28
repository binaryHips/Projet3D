#include <iostream>

#include "includes_all_cpu.h"
enum class MAP_LAYERS {
    BEDROCK,
    STONE,
    SAND,
    WATER,
    MAX_
};

int main(int, char**){
    GeoContextCPU context = GeoContextCPU::createGeoContext();
    context.update(0.1);
    //Pixel &currentPixel = context.maps[to_underlying(MAP_LAYERS::SAND)](10, 10);
    //std::cout << currentPixel << std::endl;
    std::cout << context.totalHeight(uvec2(10, 10)) << std::endl;
}