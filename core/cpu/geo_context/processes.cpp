#include "geo_context.h"


// The different processes are defined here, for now it's defined as is, but we should turn it into some kind of factory.

enum class FEATURE_LAYERS {
    DESIRED_HEIGHT,
    DESIRED_WATER,
    DESIRED_VERDURE,
    MAX_
};

enum class MAP_LAYERS {
    BEDROCK,
    STONE,
    SAND,
    WATER,
    MAX_
};

enum class ATTRIBUTE_LAYERS {
    SEDIMENT,
    MAX_
};

void processSandGravity(GeoContextCPU &context, float delta){

    for (u32 i = 0; i < IMGSIZE; ++i) for (u32 j = 0; j < IMGSIZE; ++j){
        Pixel &currentPixel = context.maps[to_underlying(MAP_LAYERS::SAND)](i, j);
        float currentHeight = context.totalHeight(uvec2(i, j));
        currentPixel += 0.1 * delta;
    }
}



GeoContextCPU GeoContextCPU::createGeoContext(){

    GeoContextCPU context = GeoContextCPU();

    context.maps.resize(to_underlying(MAP_LAYERS::MAX_));
    context.featureMaps.resize(to_underlying(FEATURE_LAYERS::MAX_));
    context.attributeMaps.resize(to_underlying(ATTRIBUTE_LAYERS::MAX_));

    context.maps[to_underlying(MAP_LAYERS::BEDROCK)].yIndex = 0;
    context.maps[to_underlying(MAP_LAYERS::STONE)].yIndex = 1;
    context.maps[to_underlying(MAP_LAYERS::SAND)].yIndex = 2;
    context.maps[to_underlying(MAP_LAYERS::WATER)].yIndex = 2;

    context.addProcess(processSandGravity);

    return context;
}