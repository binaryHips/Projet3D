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

    static void convolveToHeight(const Kernel &kernel, const GeoContextCPU & context, u32 targetMap, u32 i, u32 j){

        vec3 val;

        for (int u = -kernel.extent; u <= kernel.extent, map.sX; ++u){
            for (int v = -kernel.extent; v <= kernel.extent, map.sY; ++v){

                val += context.heightTo(uvec2(k, l), targetMap)*kernel(u, v);
            }
        }
        return val;
    };
};

namespace KernelUtilities{
    const Kernel Base_3(
        {
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0
        },
        1
    );

    const Kernel Base_7(
        {
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        },
        3
    );

    const Kernel Norm1_7(
        {
             0 ,  0 , 0  , 1.0, 0  , 0  ,  0 ,
            0  , 0  , 1.0, 1.0, 1.0, 0  , 0  ,
            0  , 1.0, 1.0, 1.0, 1.0, 1.0, 0  ,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            0  , 1.0, 1.0, 1.0, 1.0, 1.0, 0  ,
            0  , 0  , 1.0, 1.0, 1.0, 0  , 0  ,
             0 ,  0 , 0  , 1.0, 0  , 0  ,  0 
        },
        3
    );

    const Kernel test(
        {
             0 ,  1 , 2,
             3, 4, 5,
             6, 7, 8
        },
        1
    );
    const Kernel laplacian_3(
        {
            0.0, 1.0, 0.0,
            1.0, -4.0, 1.0,
            0.0, 1.0, 0.0
        },
        1
    );
    const Kernel laplacian_5(
        {
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 0.0, -4.0, 0.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0
        },
        2
    );

    /*
    https://www.youtube.com/watch?v=kOkfC5fLfgE
    at 29:52
    */

    Kernel gaussian(int extent, double smoothness);
}