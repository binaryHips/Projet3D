#include "geo_context.h"

void thermalErode(GeoContextCPU &context, float delta){

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){
        const u32 layerIndex = to_underlying(MAP_LAYERS::STONE);

        Pixel &currentPixel = context.maps[layerIndex](i, j);
        double currentHeight = context.heightTo(uvec2(i, j), layerIndex);
        if (currentHeight > 0.001)
        {
            const float maxSlope = 10.0 * (1.0 / IMGSIZE);
            
            const u32 prev_i  = (i == 0) ? 0u : i-1u;
            const u32 prev_j  = (j == 0) ? 0u : j-1u;

            // adapted from "Realtime Procedural Terrain Generation. Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games"

            // modified von neuman neighbourhood
            double grad_xy      = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(i+1, j+1), layerIndex));
            double grad_mxmy    = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(prev_i, prev_j), layerIndex));
            double grad_xmy     = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(i+1, prev_j), layerIndex));
            double grad_mxy     = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(prev_i, j+1), layerIndex));

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

void fallingSand(GeoContextCPU &context, float delta){

    delta = delta * 10.0;
    const u32 layerIndex = to_underlying(MAP_LAYERS::SAND);
    const float maxSlope = 1.0 * (1.0 / IMGSIZE);

    MapCPU& inMap = context.maps[layerIndex];
    MapCPU& outMap = context.tempMaps[layerIndex];

    for(int i = 0 ; i < IMGSIZE ; i++)
    {
        for(int j = 0 ; j < IMGSIZE ; j++)
        {
            outMap(i,j) = inMap(i,j);
        }
    }

    // Small values to decide whether the sand or height are negligeable
    double minHeight = 0.001;
    double minSand = 0.001;

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){


        Pixel &currentPixel = context.maps[layerIndex](i, j);
        double currentHeight = context.heightTo(uvec2(i, j), layerIndex);

        // delete tiny values
        if (currentHeight < minHeight) continue;
        Pixel currentSand = inMap(i, j);
        if (currentSand < minSand) continue;
        
        const u32 prev_i  = (i == 0) ? 0u : i-1u;
        const u32 prev_j  = (j == 0) ? 0u : j-1u;

        // adapted from "Realtime Procedural Terrain Generation. Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games"

        // modified von neuman neighbourhood
        double grad_xy      = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(i+1, j+1), layerIndex));
        double grad_mxmy    = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(prev_i, prev_j), layerIndex));
        double grad_xmy     = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(i+1, prev_j), layerIndex));
        double grad_mxy     = (1.0 / 2) * (currentHeight - context.heightTo(uvec2(prev_i, j+1), layerIndex));

        // use local gradient 
        // double grad_xy      = (1.0 / 2) * (currentHeight - context.maps[layerIndex](i+1, j+1));
        // double grad_mxmy    = (1.0 / 2) * (currentHeight - context.maps[layerIndex](prev_i, prev_j));
        // double grad_xmy     = (1.0 / 2) * (currentHeight - context.maps[layerIndex](i+1, prev_j));
        // double grad_mxy     = (1.0 / 2) * (currentHeight - context.maps[layerIndex](prev_i, j+1));

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
            maxgradDir_ = uvec2(prev_i, j+1);
        }

        if (maxGrad_ > maxGrad){
            maxGrad = maxGrad_;
            maxgradDir = maxgradDir_;
        }

        if (maxGrad > maxSlope){
            double maxDisplacement = std::min(maxGrad - maxSlope , currentHeight);
            float displaceToCurrent = std::clamp(maxGrad * delta, 0.0, maxDisplacement);
            outMap(i,j) -= displaceToCurrent;
            outMap(maxgradDir[0], maxgradDir[1]) += displaceToCurrent;
        }
    }

    // drain on edges
    for(int i = 0 ; i < IMGSIZE ; i++)
    {
        outMap(0, i) *= 0.95f;           // left edge
        outMap(IMGSIZE-1, i) *= 0.95f;   // right edge
        outMap(i, 0) *= 0.95f;           // top edge
        outMap(i, IMGSIZE-1) *= 0.95f;   // bottom edge    
    }


    for(int i = 0 ; i < IMGSIZE ; i++)
    {
        for(int j = 0 ; j < IMGSIZE ; j++)
        {
            inMap(i,j) = outMap(i,j);
        }
    }
}

void waterSpawnAndDrain(GeoContextCPU &context, float delta){

    delta = delta * 10.0;
    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){
        const u32 layerIndex = to_underlying(MAP_LAYERS::WATER);
        const u32 featureRainIndex = to_underlying(FEATURE_LAYERS::WATER_INFlOW);
        const u32 featureDrainIndex = to_underlying(FEATURE_LAYERS::WATER_OUTFLOW);

        Pixel& p = context.maps[layerIndex](i, j);
        p += context.featureMaps[featureRainIndex](i, j) * delta;
        p = std::max(-100.0f, p - context.featureMaps[featureDrainIndex](i, j) * delta);
    }
}


void waterMove(GeoContextCPU &context, float delta){
    const float dxdy = (1.0f);
    const float g = 10.0;
    const float cfl = dxdy / (5.0 * delta);

    const u32 layerIndex = to_underlying(MAP_LAYERS::WATER);
    const u32 velocityUIndex = to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_U);
    const u32 velocityVIndex = to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_V);

    for (u32 ii = 0; ii < IMGSIZE-1; ++ii) for (u32 jj = 0; jj < IMGSIZE-1; ++jj){



        const u32 prev_i  = (ii == 0) ? 0u : ii-1u;
        const u32 prev_j  = (jj == 0) ? 0u : jj-1u;

        Pixel &vel_u = context.attributeMaps[velocityUIndex](ii, jj);
        Pixel &vel_v = context.attributeMaps[velocityVIndex](ii, jj);


        float dh_dx = (context.heightTo(uvec2(ii, jj), layerIndex) - context.heightTo(uvec2(prev_i, jj), layerIndex) );

        float dh_dy = (context.heightTo(uvec2(ii, jj), layerIndex) - context.heightTo(uvec2(ii, prev_j), layerIndex) );

        vel_u = vel_u - std::clamp(delta * g * dh_dx, -cfl, cfl);
        vel_v = vel_v - std::clamp(delta * g * dh_dy, -cfl, cfl);

        float du_dx = ( context.attributeMaps[velocityUIndex](ii+1, jj) - context.attributeMaps[velocityUIndex](prev_i, jj) ) / (2 * dxdy);
        float dv_dy = ( context.attributeMaps[velocityVIndex](ii, jj+1) - context.attributeMaps[velocityVIndex](ii, prev_j) ) / (2 * dxdy);


        context.maps[layerIndex](ii, jj) = context.maps[layerIndex](ii, jj) + delta * (-(du_dx + dv_dy));

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

    // feature maps
    context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_HEIGHT)].name = "desired elevation";
    context.featureMaps[to_underlying(FEATURE_LAYERS::WATER_INFlOW)].name = "water inflow";
    context.featureMaps[to_underlying(FEATURE_LAYERS::WATER_OUTFLOW)].name = "water sink";

    // fill tmp uffer with maps
    context.tempMaps.resize(context.maps.size());
    for(size_t i = 0 ; i < context.maps.size() ; ++i)
    {
        context.tempMaps[i] = context.maps[i];
    } 

    context.addProcess(fallingSand);
    // context.addProcess(waterSpawnAndDrain);
    // context.addProcess(waterMove);
    return context;
}
