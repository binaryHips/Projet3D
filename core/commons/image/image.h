#pragma once
#include "defines.h"
#include <vector>

class ImageBase__ {

    u32 sX, sY;

    std::vector<Pixel, AlignmentAllocator<T, 512 / sizeof(Pixel)> > data;

    Pixel operator() (const u32 x, const u32 y) DEBUG_EXCEPT {

        return data[sX + sY * SX]
    }

}