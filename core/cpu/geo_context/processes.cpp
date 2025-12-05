#include "geo_context.h"



void thermalErode(GeoContextCPU &context, float delta){

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){
        u32 layerIndex = to_underlying(MAP_LAYERS::SAND);

        Pixel &currentPixel = context.maps[layerIndex](i, j);
        double currentHeight = context.heightTo(uvec2(i, j), layerIndex);

        const float maxSlope = 0.1 * (1.0 / IMGSIZE);
        const double maxDisplaceQuantity = 0.5; // x times the difference in height.
        
        // adapted from "Realtime Procedural Terrain Generation. Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games"

        // modified von neuman neighbourhood
        double grad_xy      = currentHeight - context.heightTo(uvec2(i+1, j+1), layerIndex);
        double grad_mxmy    = currentHeight - context.heightTo(uvec2(i-1, j-1), layerIndex);
        double grad_xmy     = currentHeight - context.heightTo(uvec2(i+1, j-1), layerIndex);
        double grad_mxy     = currentHeight - context.heightTo(uvec2(i-1, j+1), layerIndex);

        double maxGrad = 0.0;
        double maxGrad_ = 0.0; // used for small speedup
        uvec2 maxgradDir;
        uvec2 maxgradDir_;


        // find max dir 
        if (grad_xy > grad_mxmy){
            maxGrad = grad_xy;
            maxgradDir = uvec2(i+1, j+1);
        } else {
            maxGrad = grad_mxmy;
            maxgradDir = uvec2(i-1, j-1);
        }
        if (grad_xmy > grad_mxy){
            maxGrad_ = grad_xy;
            maxgradDir_ = uvec2(i+1, j-1);
        } else {
            maxGrad = grad_xy;
            maxgradDir = uvec2(i-1, j+1);
        }

        if (maxGrad_ > maxGrad){
            maxGrad = maxGrad_;
            maxgradDir = maxgradDir_;
        }

        if (maxGrad > maxSlope){
            float displaceToCurrent = std::min(maxGrad * delta, maxGrad / 2.0);
            currentPixel -= displaceToCurrent;
            context.maps[layerIndex](maxgradDir[0], maxgradDir[1]) += displaceToCurrent;
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

    context.addProcess(thermalErode);

    return context;
}
