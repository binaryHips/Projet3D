#pragma once
#include "defines.h"
#include "maths/maths.h"
#include "commons/allocator.h"
#include <vector>

class MapBase__ {
public:
    // The "height" of the layer represented by the map. With the following behavior :
    // - we're always over the layers with an inferior yIndex
    // - if another layer has the same yIndex, the highest one wins.
    u32 yIndex = 0;
    vec3 color = vec3(1.0f);
};