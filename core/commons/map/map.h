#pragma once
#include "defines.h"
#include <vector>

class MapBase__ {

    u32 sX, sY;

    // The "height" of the layer represented by the map. With the following behavior :
    // - we're always over the layers with an inferior yIndex
    // - if another layer has the same yIndex, the highest one wins.
    u32 yIndex = 0;

    std::vector<Pixel, AlignmentAllocator<T, 512 / sizeof(Pixel)> > data;

    Pixel operator() (const u32 x, const u32 y) const DEBUG_EXCEPT {
        return data[sX + sY * SX];
    }

    Pixel& operator() (const u32 x, const u32 y) DEBUG_EXCEPT {
        return data[sX + sY * SX];
    }

    Pixel operator() (uvec2) DEBUG_EXCEPT {
        return data[sX + sY * SX];
    }
};