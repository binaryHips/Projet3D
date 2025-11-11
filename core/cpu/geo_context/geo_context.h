#pragma once
#include <algorithm>
#include <memory>

#include "commons/geo_context/geo_context.h"
#include "cpu/map/map.h"
#include "cpu/particle_system/particle_system.h"

class GeoContextCPU : public GeoContextBase__ {
public:
    // std::vector<FeatureMap> featureMaps;

    ParticleSystemCPU particleSystem;

    std::vector<MapCPU> maps;

    void addMap(MapCPU &&map){
        for (u32 i = 0; i < maps.size(); ++i){
            if (maps[i].yIndex > map.yIndex){
                maps.insert(maps.begin() + i, std::move(map));
                return;
            }
        }
    }

    float totalHeight(uvec2 pos){

        float height = 0;
        float currentYIndexHeight = 0;
        u32 currentYIndex = 0;

        for (auto &map : maps){

            Pixel h =  map(pos);

            if (currentYIndex == map.yIndex){
                currentYIndexHeight = std::max(currentYIndexHeight, h);
            } else {
                height += currentYIndexHeight;
                currentYIndex++;
                currentYIndexHeight = h;
            }
        }
        return height;
    }
};