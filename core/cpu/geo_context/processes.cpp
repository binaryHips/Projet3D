#include "geo_context.h"



void thermalErode(GeoContextCPU &context, float delta){

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){
        u32 layerIndex = to_underlying(MAP_LAYERS::SAND);

        Pixel &currentPixel = context.maps[layerIndex](i, j);
        double currentHeight = context.heightTo(uvec2(i, j), layerIndex);
        if (currentHeight > 0.001)
        {
            const float maxSlope = 1.0 * (1.0 / IMGSIZE);
            
            u32 prev_i  = (i == 0) ? 0u : i-1u;
            u32 prev_j  = (j == 0) ? 0u : j-1u;

            // adapted from "Realtime Procedural Terrain Generation. Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games"

            // modified von neuman neighbourhood
            double grad_xy      = currentHeight - context.heightTo(uvec2(i+1, j+1), layerIndex);
            double grad_mxmy    = currentHeight - context.heightTo(uvec2(prev_i, prev_j), layerIndex);
            double grad_xmy     = currentHeight - context.heightTo(uvec2(i+1, prev_j), layerIndex);
            double grad_mxy     = currentHeight - context.heightTo(uvec2(prev_i, j+1), layerIndex);

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
                maxgradDir = uvec2(prev_i, prev_j);
            }
            if (grad_xmy > grad_mxy){
                maxGrad_ = grad_xmy;
                maxgradDir_ = uvec2(i+1, prev_j);
            } else {
                maxGrad_ = grad_mxy;
                maxgradDir = uvec2(prev_i, j+1);
            }

            if (maxGrad_ > maxGrad){
                maxGrad = maxGrad_;
                maxgradDir = maxgradDir_;
            }

            if (maxGrad > maxSlope){
                float displaceToCurrent = std::min(maxGrad * delta, currentHeight);
                currentPixel -= displaceToCurrent;
                context.maps[layerIndex](maxgradDir[0], maxgradDir[1]) += displaceToCurrent;
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

    // materials index for the shader (fow now same indices)

    context.maps[to_underlying(MAP_LAYERS::BEDROCK)].materialIndex = to_underlying(MAP_LAYERS::BEDROCK);
    context.maps[to_underlying(MAP_LAYERS::STONE)].materialIndex = to_underlying(MAP_LAYERS::STONE);
    context.maps[to_underlying(MAP_LAYERS::SAND)].materialIndex = to_underlying(MAP_LAYERS::SAND);
    context.maps[to_underlying(MAP_LAYERS::WATER)].materialIndex = to_underlying(MAP_LAYERS::WATER);

    context.maps[to_underlying(MAP_LAYERS::BEDROCK)].name = "Bedrock";
    context.maps[to_underlying(MAP_LAYERS::STONE)].name = "Stone";
    context.maps[to_underlying(MAP_LAYERS::SAND)].name = "Sand";
    context.maps[to_underlying(MAP_LAYERS::WATER)].name = "Water";

    context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_HEIGHT)].name = "Height";
    context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_WATER)].name = "Water";
    context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_VERDURE)].name = "Verdure";




    context.addProcess(thermalErode);

    return context;
}
