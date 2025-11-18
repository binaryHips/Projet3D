#pragma once
#include "defines.h"
#include "maths/maths.h"
#include "commons/allocator.h"
#include <vector>

class MapBase__ {
public:
    u32 sX, sY;

    // The "height" of the layer represented by the map. With the following behavior :
    // - we're always over the layers with an inferior yIndex
    // - if another layer has the same yIndex, the highest one wins.
    u32 yIndex = 0;

    std::vector<Pixel, AlignmentAllocator<Pixel, 512 / sizeof(Pixel)> > data;


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