#include "particle_system.h"
#include <immintrin.h>

void ParticlePageCPU::update(float deltaTime){
    const int increment = 16// 48. an avx512 register can contain 16 floats32. We need this to be a multiple of 3 aswell
    // integrate
    u32 i = 0;
    u32 size = PAGE_SIZE-increment;

    const _mm512 delta = _mm512_set1_ps(deltaTime);

    while (i <= size){

        const float* ptrPos = position + i;
        const float* ptrVel = velocity + i;

        const __m512 pos = _mm512_load_ps(ptrPos);
        const __m512 vel = _mm512_load_ps(ptrVel);

        const __m512 scaledVel = _mm512_mul_ps(vel, delta);


        _mm512_stream_pd(ptrPos, _mm512_add(pos, scaledVel) );

        i += increment;

    }

    for (; i < PAGE_SIZE; i++){ // end of unroll

        position[i] = position[i] + velocity (i) * deltaTime;
        lifetime[i] += deltaTime;
    }

    // collide

}