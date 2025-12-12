#include "geo_context.h"

void thermalErode(GeoContextCPU &context, float delta){

    const u32 layerIndex = to_underlying(MAP_LAYERS::STONE);
    const float maxSlope = 2.0 * (1.0 / IMGSIZE);

    MapCPU& inMap = context.maps[layerIndex];
    MapCPU outMap = inMap;

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

    context.maps[layerIndex] = std::move(outMap);
}

void tectonics(GeoContextCPU &context, float delta){

    delta = delta * 0.1;
    const u32 layerIndex = to_underlying(MAP_LAYERS::STONE);
    const u32 layerIndex2 = to_underlying(MAP_LAYERS::BEDROCK);

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j){


        Pixel &currentPixel = context.maps[layerIndex](i, j);

        float desiredHeight = context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_HEIGHT)](i, j) / 2.0;

        currentPixel += (desiredHeight - currentPixel) * delta;

        Pixel &currentPixel2 = context.maps[layerIndex2](i, j);

        currentPixel2 += (desiredHeight - currentPixel2) * delta;
    }
}

void fallingSand(GeoContextCPU &context, float delta){

    delta = delta * 10.0;
    const u32 layerIndex = to_underlying(MAP_LAYERS::SAND);
    const float maxSlope = 1.0 * (1.0 / IMGSIZE);

    MapCPU& inMap = context.maps[layerIndex];
    MapCPU outMap = inMap;

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

    context.maps[layerIndex] = std::move(outMap);
}

void sandCalcification(GeoContextCPU &context , float delta)
{

    const u32 sandLayerIndex = to_underlying(MAP_LAYERS::SAND);
    const u32 stoneLayerIndex = to_underlying(MAP_LAYERS::STONE);

    float threshold = 0.2;

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j)
    {
        float calcifiedQuantity = std::max(delta * 0.05 , 0.000001); // probably gonna be uge but wharreva

        Pixel &sandPixel = context.maps[sandLayerIndex](i, j);
        Pixel &stonePixel = context.maps[stoneLayerIndex](i, j);

        // if(sandPixel < 0 || calcifiedQuantity < 0)
        //     std::cout << "Calc : " << calcifiedQuantity << "sand : " << sandPixel << std::endl ;


        calcifiedQuantity = std::clamp(calcifiedQuantity, 0.0f , sandPixel);

        if(sandPixel > threshold)
        {
            sandPixel -= calcifiedQuantity;
            stonePixel += calcifiedQuantity;
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



void waterMove(GeoContextCPU &context, float delta) {
    const float dxdy = 1.0f;
    const float g = 9.81f;
    const float dryThreshold = 0.001f;
    
    const u32 layerIndex = to_underlying(MAP_LAYERS::WATER);
    const u32 velocityUIndex = to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_U);
    const u32 velocityVIndex = to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_V);

    MapCPU& h = context.maps[layerIndex];  
    MapCPU& u = context.attributeMaps[velocityUIndex];  
    MapCPU& v = context.attributeMaps[velocityVIndex]; 
    
    MapCPU newH = h;
    MapCPU newU = u;
    MapCPU newV = v;

    // Update velocities
    for (u32 i = 1; i < IMGSIZE - 1; ++i) {
        for (u32 j = 1; j < IMGSIZE - 1; ++j) {
            
            float heightCenter = h(i, j);
            
            if (heightCenter < dryThreshold) {
                newU(i, j) = 0.0f;
                newV(i, j) = 0.0f;
                continue;
            }
            
            // gradients like arthur

            float heightLeft = h(i - 1, j);
            float heightRight = h(i + 1, j);
            float heightBottom = h(i, j - 1);
            float heightTop = h(i, j + 1);

            float terrainCenter = context.heightTo(uvec2(i,j), layerIndex);
            float terrainLeft   = context.heightTo(uvec2(i-1,j), layerIndex);
            float terrainRight  = context.heightTo(uvec2(i+1,j), layerIndex);
            float terrainBottom = context.heightTo(uvec2(i,j-1), layerIndex);
            float terrainTop    = context.heightTo(uvec2(i,j+1), layerIndex);
            
            float etaCenter = terrainCenter + heightCenter;
            float etaLeft   = terrainLeft + heightLeft;
            float etaRight  = terrainRight + heightRight;
            float etaBottom = terrainBottom + heightBottom;
            float etaTop    = terrainTop + heightTop;
                        
            float dh_dx = (etaRight - etaLeft) / (2.0f * dxdy);
            float dh_dy = (etaTop - etaBottom) / (2.0f * dxdy);
            
            float centerU = u(i, j);
            float centerV = v(i, j);
            
            float du_dx, dv_dy;
            
            // upwind for u
            if (centerU > 0.0f) {
                du_dx = (centerU - u(i - 1, j)) / dxdy;
            } else {
                du_dx = (u(i + 1, j) - centerU) / dxdy;
            }
            
            // upwind for v
            if (centerV > 0.0f) {
                dv_dy = (centerV - v(i, j - 1)) / dxdy;
            } else {
                dv_dy = (v(i, j + 1) - centerV) / dxdy;
            }
            
            // momentuim
            float du_dt = -centerU * du_dx - g * dh_dx;
            float dv_dt = -centerV * dv_dy - g * dh_dy;
            
            float viscosity = 0.5f;
            float laplacianU = (u(i+1,j) + u(i-1,j) + u(i,j+1) + u(i,j-1) - 4.0f*centerU) / (dxdy*dxdy);
            float laplacianV = (v(i+1,j) + v(i-1,j) + v(i,j+1) + v(i,j-1) - 4.0f*centerV) / (dxdy*dxdy);
            
            du_dt += viscosity * laplacianU;
            dv_dt += viscosity * laplacianV;
            
            // update velocities
            newU(i, j) = centerU + du_dt * delta;
            newV(i, j) = centerV + dv_dt * delta;
            
            float max_speed = std::sqrt(g * heightCenter);
            float speed_limit = 2.0f * max_speed;  
            
            float speed = std::sqrt(newU(i,j)*newU(i,j) + newV(i,j)*newV(i,j));
            if (speed > speed_limit) {
                float scale = speed_limit / speed;
                newU(i, j) *= scale;
                newV(i, j) *= scale;
            }
        }
    }
    
    // edge cases (merci arthur)
    for (u32 i = 0; i < IMGSIZE; ++i) {
        newU(i, 0) = 0.0f;
        newV(i, 0) = 0.0f;
        newU(i, IMGSIZE-1) = 0.0f;
        newV(i, IMGSIZE-1) = 0.0f;
    }
    for (u32 j = 0; j < IMGSIZE; ++j) {
        newU(0, j) = 0.0f;
        newV(0, j) = 0.0f;
        newU(IMGSIZE-1, j) = 0.0f;
        newV(IMGSIZE-1, j) = 0.0f;
    }
    
    // Update water
    for (u32 i = 1; i < IMGSIZE - 1; ++i) {
        for (u32 j = 1; j < IMGSIZE - 1; ++j) {
            
            float heightCenter = h(i, j);
            
            // Compute flux at right face (between i,j and i+1,j)
            float h1 = h(i, j);
            float h2 = h(i+1, j);
            float u1 = newU(i, j);
            float u2 = newU(i+1, j);
            
            float fluxRight;
            if (h1 < dryThreshold && h2 < dryThreshold) {
                fluxRight = 0.0f;
            } else if (h1 < dryThreshold) {
                fluxRight = h2 * u2;
            } else if (h2 < dryThreshold) {
                fluxRight = h1 * u1;
            } else {
                fluxRight = 0.5f * (h1 + h2) * 0.5f * (u1 + u2);
            }
            
            // Compute flux at left face (between i-1,j and i,j)
            h1 = h(i-1, j);
            h2 = h(i, j);
            u1 = newU(i-1, j);
            u2 = newU(i, j);
            
            float fluxLeft;
            if (h1 < dryThreshold && h2 < dryThreshold) {
                fluxLeft = 0.0f;
            } else if (h1 < dryThreshold) {
                fluxLeft = h2 * u2;
            } else if (h2 < dryThreshold) {
                fluxLeft = h1 * u1;
            } else {
                fluxLeft = 0.5f * (h1 + h2) * 0.5f * (u1 + u2);
            }
            
            // Compute flux at top face (between i,j and i,j+1)
            h1 = h(i, j);
            h2 = h(i, j+1);
            float v1 = newV(i, j);
            float v2 = newV(i, j+1);
            
            float fluxTop;
            if (h1 < dryThreshold && h2 < dryThreshold) {
                fluxTop = 0.0f;
            } else if (h1 < dryThreshold) {
                fluxTop = h2 * v2;
            } else if (h2 < dryThreshold) {
                fluxTop = h1 * v1;
            } else {
                fluxTop = 0.5f * (h1 + h2) * 0.5f * (v1 + v2);
            }
            
            // Compute flux at bottom face (between i,j-1 and i,j)
            h1 = h(i, j-1);
            h2 = h(i, j);
            v1 = newV(i, j-1);
            v2 = newV(i, j);
            
            float fluxBottom;
            if (h1 < dryThreshold && h2 < dryThreshold) {
                fluxBottom = 0.0f;
            } else if (h1 < dryThreshold) {
                fluxBottom = h2 * v2;
            } else if (h2 < dryThreshold) {
                fluxBottom = h1 * v1;
            } else {
                fluxBottom = 0.5f * (h1 + h2) * 0.5f * (v1 + v2);
            }
            
            float div_x = (fluxRight - fluxLeft) / dxdy;
            float div_y = (fluxTop - fluxBottom) / dxdy;
            
            float divergence = div_x + div_y;
            
            float dh_dt = -divergence;
            
            newH(i, j) = heightCenter + dh_dt * delta;
            newH(i, j) = std::max(0.0f, newH(i, j));
        }
    }
    
    context.maps[layerIndex] = std::move(newH);
    context.attributeMaps[velocityUIndex] = std::move(newU);
    context.attributeMaps[velocityVIndex] = std::move(newV);

    float totalHeight = 0;
    for (u32 i = 1; i < IMGSIZE - 1; ++i) {
        for (u32 j = 1; j < IMGSIZE - 1; ++j) {
            totalHeight += context.maps[layerIndex](i,j);
        }
    }

    std::cout << "Total height after process : " << totalHeight<< std::endl;
}

void sandStorm(GeoContextCPU &context, float delta){

    const int n_pages = 1;
    const float spawnDelay = 0.5; const float lifetime = 5.0;
    const vec3 winddirection  = vec3(0.35, 0, 0); const vec3 gravity = winddirection + vec3(0, -0.1, 0); const float sandValue = 0.1;
    static float time = spawnDelay; time += delta;

    if (time > spawnDelay){
        time = 0.0;
        context.particleSystem.spawn(n_pages, lifetime, spawnDelay /*does nothing for now*/, winddirection, gravity,  0.0, sandValue);
    }
}

void wind(GeoContextCPU &context, float delta){

    const int n_pages = 1;
    const float spawnDelay = 0.5;
    const float lifetime = 5.0;
    const vec3 winddirection  = vec3(0.35, - 0.01, 0);
    const vec3 gravity = winddirection + vec3(0, -0.2, 0);

    static float time = spawnDelay;
    time += delta;

    if (time > spawnDelay){
        time = 0.0;
        context.particleSystem.spawn(n_pages, lifetime, spawnDelay /*does nothing for now*/, winddirection, gravity,  0.0, 0.0);
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
    context.featureMaps[to_underlying(FEATURE_LAYERS::DESIRED_HEIGHT)].name = "Desired elevation";
    context.featureMaps[to_underlying(FEATURE_LAYERS::WATER_INFlOW)].name = "Water inflow";
    context.featureMaps[to_underlying(FEATURE_LAYERS::WATER_OUTFLOW)].name = "Water sink";

    for (u32 i = 0; i < IMGSIZE-1; ++i) for (u32 j = 0; j < IMGSIZE-1; ++j)
    {
        context.attributeMaps[to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_U)](i,j) = 0.1f;
        context.attributeMaps[to_underlying(ATTRIBUTE_LAYERS::WATER_VELOCITY_V)](i,j) = 0.1f;
    }
    context.addProcess(ProcessCPU(thermalErode, "Erosion"));
    context.addProcess(ProcessCPU(tectonics, "Tectonics"));
    context.addProcess(ProcessCPU(fallingSand, "Sand"));
    context.addProcess(ProcessCPU(sandCalcification, "Cementation"));
    context.addProcess(ProcessCPU(wind, "Wind"));
    context.addProcess(ProcessCPU(waterSpawnAndDrain, "Rain and drain"));
    context.addProcess(ProcessCPU(sandStorm, "Sandstorm"));
    context.addProcess(ProcessCPU(waterMove, "Shallow water"));
    return context;
}
