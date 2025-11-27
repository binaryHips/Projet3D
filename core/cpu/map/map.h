#pragma once
#include "commons/map/map.h"
#include <algorithm>

class MapCPU : public MapBase__{

    std::vector<Pixel, AlignmentAllocator<Pixel, 512 / sizeof(Pixel)> > data;



public:

    MapCPU(){
        data = std::vector<Pixel, AlignmentAllocator<Pixel, 512 / sizeof(Pixel)> >(IMGSIZE * IMGSIZE, 0);
    }

    inline Pixel operator() (const u32 x, const u32 y) const noexcept{
        u32 clamped_x = std::clamp(x, 0u, IMGSIZE);
        u32 clamped_y = std::clamp(y, 0u, IMGSIZE);
        return data[clamped_x + clamped_y * sX];
    }
    inline Pixel& operator() (const u32 x, const u32 y) noexcept {
        u32 clamped_x = std::clamp(x, 0u, IMGSIZE);
        u32 clamped_y = std::clamp(y, 0u, IMGSIZE);
        return data[clamped_x + clamped_y * sX];
    }

    inline Pixel operator() (const uvec2 v) const noexcept {
        u32 clamped_x = std::clamp(v[0], 0u, IMGSIZE);
        u32 clamped_y = std::clamp(v[1], 0u, IMGSIZE);
        return data[clamped_x + clamped_y * sX];
    }
    inline Pixel& operator() (const uvec2 v) noexcept {
        u32 clamped_x = std::clamp(v[0], 0u, IMGSIZE);
        u32 clamped_y = std::clamp(v[1], 0u, IMGSIZE);
        return data[clamped_x + clamped_y * sX];
    }

};