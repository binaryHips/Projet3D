#pragma once
#include <algorithm>
#include <memory>
#include <map>

#include "commons/geo_context/geo_context.h"
#include "cpu/map/map.h"
#include "cpu/particle_system/particle_system.h"

class GeoContextCPU : public GeoContextBase__ {

private:
    GeoContextCPU() = default;
public:
    using Process = void(*)(GeoContextCPU&, float);
    std::vector<MapCPU> featureMaps; // maps that drive the processes

    ParticleSystemCPU particleSystem = ParticleSystemCPU(this);

    std::vector<MapCPU> maps; // base heightmaps that will be used for terrain generation

    std::vector<MapCPU> attributeMaps; // maps used for internal working of physical processes. (ex : sediment map for hydro erosion)

    std::vector<Process> processes; // functions that will update the maps

    void update(float delta){

        for (Process process: processes){
            process(*this, delta);
        }

        particleSystem.update(delta);
    }

    void addProcess(Process process){
        processes.push_back(process);
    }

    void addMap(MapCPU &&map){
        // Insert map keeping ordering by yIndex. If no insertion happened, append at the end.
        for (u32 i = 0; i < maps.size(); ++i){
            if (maps[i].yIndex > map.yIndex){
                maps.insert(maps.begin() + i, std::move(map));
                return;
            }
        }
        maps.push_back(std::move(map));
    }

    float totalHeight(float x, float y) const {

        // TODO unneceessary conversion
        uvec2 pxVec = uvec2(x * IMGSIZE, y * IMGSIZE);
        return totalHeight(pxVec);
    }

    inline float totalHeight(uvec2 pos) const {

        float height = 0.0;
        float currentYIndexHeight = 0.0;
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
        return height + currentYIndexHeight; // add the last one
    }

    inline float heightTo(uvec2 pos, u32 targetMapIndex) const {

        float height = 0.0;
        float currentYIndexHeight = 0.0;
        u32 currentYIndex = 0;

        for (u32 i = 0; i < maps.size(); ++i){
            const MapCPU &map  = maps[i];
            Pixel h =  map(pos);

            if (i >= targetMapIndex){
                currentYIndexHeight = h;
                break;
            }

            if (currentYIndex == map.yIndex){
                currentYIndexHeight = std::max(currentYIndexHeight, h);
            } else {
                height += currentYIndexHeight;
                currentYIndex++;
                currentYIndexHeight = h;
            }
        }
        return height + currentYIndexHeight; // add the last one
    }

    static GeoContextCPU createGeoContext();
};
