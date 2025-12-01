#include "particle_system.h"
#include <immintrin.h>
#include "cpu/geo_context/geo_context.h"
void ParticlePageCPU::update(float deltaTime, const GeoContextCPU &context){
    const int increment = 16 * 3; // 16 x 4 bytes = 512 bits

    // integrate
    u32 i = 0;
    u32 size = nbParticles-increment;

    const __m512 delta = _mm512_set1_ps(deltaTime);

    const vec3 gravity = context.settings.gravity;
    const __m512 gravityExpanded[3] = { // expand as virtual stream
        _mm512_set_ps(  gravity[0], gravity[1], gravity[2], gravity[0],
                        gravity[1], gravity[2], gravity[0], gravity[1],
                        gravity[2], gravity[0], gravity[1], gravity[2],
                        gravity[0], gravity[1], gravity[2], gravity[0]
                    ),
        _mm512_set_ps(  gravity[1], gravity[2], gravity[0], gravity[1],
                        gravity[2], gravity[0], gravity[1], gravity[2],
                        gravity[0], gravity[1], gravity[2], gravity[0],
                        gravity[1], gravity[2], gravity[0], gravity[1]
                    ),
        _mm512_set_ps(  gravity[2], gravity[0], gravity[1], gravity[2],
                        gravity[0], gravity[1], gravity[2], gravity[0],
                        gravity[1], gravity[2], gravity[0], gravity[1],
                        gravity[2], gravity[0], gravity[1], gravity[2]
                    )
    };
    const __m512 scaledGrav[3] = { 
        _mm512_mul_ps(gravityExpanded[0], delta),
        _mm512_mul_ps(gravityExpanded[1], delta),
        _mm512_mul_ps(gravityExpanded[2], delta)
    };

    while (i <= size){

        float *ptrPos = reinterpret_cast<float *>(position) + i;
        float *ptrVel =  reinterpret_cast<float *>(velocity) + i;

        // update velocities and positions
        // We have to do 3 at the same time since it's vec3s, so the scalar attributes stay aligned with the flow
        for (u32 offset = 0; offset < 3; offset++){
            const __m512 pos = _mm512_load_ps(ptrPos + offset * 16);
            const __m512 vel = _mm512_load_ps(ptrVel+ offset * 16);

            const __m512 updatedVel = _mm512_add_ps(pos, scaledGrav[offset]);

            const __m512 scaledVel = _mm512_mul_ps(vel, delta);
            const __m512 updatedPos = _mm512_add_ps(pos, scaledVel);
            _mm512_stream_ps(ptrPos, updatedPos);
        }

        // update lifetime
        float *ptrLifetime =  reinterpret_cast<float *>(lifetime) + i;
        const __m512 lifetimes = _mm512_load_ps(ptrLifetime);
        _mm512_stream_ps(ptrLifetime, _mm512_mul_ps(lifetimes, delta));

        i += increment;
    }

    for (; i < nbParticles; i++){ // end of unroll
        
        position[i] = position[i] + velocity[i] * deltaTime;
        lifetime[i] += deltaTime;
    }

    // collide

}