#pragma once
#include "commons/map/map.h"
#include <algorithm>

class MapCPU : public MapBase__{

    std::vector<Pixel> data;

public:

    MapCPU(){
        data = std::vector<Pixel>(IMGSIZE * IMGSIZE, 0.0f);
        std::cout << &data << std::endl;
    }

    inline Pixel operator() (const u32 x, const u32 y) const {
        u32 clamped_x = std::clamp(x, 0u, IMGSIZE);
        u32 clamped_y = std::clamp(y, 0u, IMGSIZE);
        return data[clamped_x + clamped_y * IMGSIZE];
    }
    inline Pixel& operator() (const u32 x, const u32 y) {
        u32 clamped_x = std::clamp(x, 0u, IMGSIZE);
        u32 clamped_y = std::clamp(y, 0u, IMGSIZE);
        return data[clamped_x + clamped_y * IMGSIZE];
    }

    inline Pixel operator() (const uvec2 v) const{
        u32 clamped_x = std::clamp(v[0], 0u, IMGSIZE);
        u32 clamped_y = std::clamp(v[1], 0u, IMGSIZE);
        std::cout << "YOLO " << &data << std::endl;
        return data[clamped_x + clamped_y * IMGSIZE];
    }
    inline Pixel& operator() (const uvec2 v) {
        u32 clamped_x = std::clamp(v[0], 0u, IMGSIZE);
        u32 clamped_y = std::clamp(v[1], 0u, IMGSIZE);
        return data[clamped_x + clamped_y * IMGSIZE];
    }

};
