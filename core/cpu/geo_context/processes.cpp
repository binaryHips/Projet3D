#include "geo_context.h"



void processSandGravity(GeoContextCPU &context, float delta){

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){
        u32 sandLayerIndex = to_underlying(MAP_LAYERS::SAND);

        Pixel &currentPixel = context.maps[sandLayerIndex](i, j);
        float currentHeight = context.heightTo(uvec2(i+1, j), sandLayerIndex);


        const float max_slope = 0.4;
        const float maxDisplaceQuantity = 1.0; // x times the difference in height.
        
        // adapted from "Realtime Procedural Terrain Generation. Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games"
        float grad_x = context.heightTo(uvec2(i+1, j), sandLayerIndex) - currentHeight;
        float grad_y = context.heightTo(uvec2(i, j+1), sandLayerIndex) - currentHeight;

        if (abs(grad_x) > max_slope | abs(grad_y) > max_slope){
            if (abs(grad_x) > abs(grad_y)){
                float displaceToCurrent = maxDisplaceQuantity * grad_x * delta;
                currentPixel += displaceToCurrent;
                context.maps[sandLayerIndex](i+1, j) -= displaceToCurrent;
            } else {
                float displaceToCurrent = maxDisplaceQuantity * grad_y * delta;
                currentPixel += displaceToCurrent;
                context.maps[sandLayerIndex](i, j+1) -= displaceToCurrent;
            }
        } 
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
