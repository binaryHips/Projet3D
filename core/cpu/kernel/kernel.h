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
                for (int   u = -kernel.extent; u <= std::min(kernel.extent, map.sX - i - 1); ++u){ // don't go above size. Branchless.
                    for (int   v = -kernel.extent; v <=std::min(kernel.extent, map.sX - i - 1); ++v){
                        
                        int k = i+u;
                        int l = j+v;

                        val += map(k, l)*kernel(u, v);
                    }
                    //std::cout << "\n";
                }
                result(i, j) = val;
            }
        }
        return result;

    }
}