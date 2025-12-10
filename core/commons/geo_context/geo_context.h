#pragma once

#include "cpu/map/map.h"
#include "cpu/particle_system/particle_system.h"

struct GeoSettings{
    float mapScale = 1.0; // pixel to meters
    vec3 gravity = vec3(0, -1, 0);
};

class GeoContextBase__ {
public:
    GeoSettings settings;
    // std::vector<FeatureMap> featureMaps;
};