#pragma once

#include "cpu/map/map.h"
#include "cpu/particle_system/particle_system.h"

struct GeoSettings{
    vec3 gravity = vec3(0, -1, 0);
};

class GeoContextBase__ {
    GeoSettings settings;
    // std::vector<FeatureMap> featureMaps;
};