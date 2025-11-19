#pragma once
#include "commons/map/map.h"

class MapCPU : public MapBase__{

    std::vector<Pixel, AlignmentAllocator<Pixel, 512 / sizeof(Pixel)> > data;

public:
#if FMDEBUG
    Pixel operator() (const u32 x, const u32 y) const {
        return data.at(x + y * sX);
    }
    Pixel& operator() (const u32 x, const u32 y) {
        return data.at(x + y * sX);
    }

    Pixel operator() (uvec2 v) const {
        return data.at(v[0] + v[1] * sX);
    }
    Pixel& operator() (uvec2 v) {
        return data.at(v[0] + v[1] * sX);
    }

#else

    inline Pixel operator() (const u32 x, const u32 y) const noexcept{
        return data[x + y * sX];
    }
    inline Pixel& operator() (const u32 x, const u32 y) noexcept {
        return data[x + y * sX];
    }

    inline Pixel operator() (uvec2 v) const noexcept {
        return data[v[0] + v[1] * sX];
    }
    inline Pixel& operator() (uvec2 v) noexcept {
        return data[v[0]+ v[1] * sX];
    }

#endif
};