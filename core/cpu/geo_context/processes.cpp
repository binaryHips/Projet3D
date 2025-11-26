#include "geo_context.h"


// gives variables i, j, currentPixel (ref, val on current map at (i, j)) and currentHeight (total height at (i, j))

#define FOREACH(context, map) for (u32 i = 0; i < IMGSIZE; ++i) for (u32 j = 0, Pixel &currentPixel = map(i, j), float currentHeight = context.totalHeight(uvec2(i, j)); j < IMGSIZE; ++j, Pixel &currentPixel = map(i, j), float currentHeight = context.totalHeight(uvec2(i, j)))

// The different processes are defined here, for now it's defined as is, but we should turn it into some kind of factory.

enum class FEATURE_LAYERS = {
    DESIRED_HEIGHT,
    DESIRED_WATER,
    DESIRED_VERDURE,
    MAX_
};

enum class MAP_LAYERS = {
    BEDROCK,
    STONE,
    SAND,
    WATER,
    MAX_
};

enum class ATTRIBUTE_LAYERS = {
    SEDIMENT,
    MAX_
};

void processSandGravity(GeoContextCPU &context){
    FOREACH(context, context.maps[MAP_LAYERS.SAND]){

    }
}



GeoContextCPU GeoContextCPU::create_geo_context(){

    GeoContextCPU context;

    context.maps.resize(MAP_LAYERS.MAX_);
    context.featureMaps.resize(FEATURE_LAYERS.MAX_);
    context.attributeMaps.resize(ATTRIBUTE_LAYERS.MAX_);

    context.maps[MAP_LAYERS.BEDROCK] = 0;
    context.maps[MAP_LAYERS.STONE] = 1;
    context.maps[MAP_LAYERS.SAND] = 2;
    context.maps[MAP_LAYERS.WATER] = 2;



}