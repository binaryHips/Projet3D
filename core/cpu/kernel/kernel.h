#include "cpu/map/map.h"
#include <immintrin.h>
#include <algorithm>

template <u8 extent>
class  Kernel {
    float data[extent * extent] = {0};
    
    float& operator() (int x, int y) {
        return data[(x+extent) + (y+extent) * (2 * extent + 1)];
    }
    float operator() (int x, int y) const {
        return data[(x+extent) + (y+extent) * (2 * extent + 1)];
    }

    static void convolve(const Kernel &kernel, MapCPU &map){
        
        // TODO if this does not get giorgized by the compiler, then giorgize it

        for (u32 i = 0; i < map.sX; ++i){
            for (u32 j = 0; j < map.sY; ++j){

                vec3 val;

                for (int u = -kernel.extent; u <= kernel.extent, map.sX; ++u){
                    for (int v = -kernel.extent; v <= kernel.extent, map.sY; ++v){
                        
                        int k = std::clamp(i+u, 0, map.sX);
                        int l = std::clamp(j+v, 0, map.sY);

                        val += map(k, l)*kernel(u, v);
                    }
                }
                result(i, j) = val;
            }
        }
        return result;
    }
};