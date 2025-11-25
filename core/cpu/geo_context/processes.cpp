#include "geo_context.h"


// The different processes are defined here, for now it's defined as is, but we should turn it into some kind of factory.

enum FEATURE_LAYERS = {
    DESIRED_HEIGHT,
    DESIRED_WATER,
    DESIRED_VERDURE
}

enum MAP_LAYERS = {
    BEDROCK,
    STONE,
    SAND,
    WATER
}

enum ATTRIBUTE_LAYERS = {
    
}


GeoContextCPU GeoContextCPU::create_geo_context(){

    GeoContextCPU context;

    
}