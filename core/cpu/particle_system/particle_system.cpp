#include "particle_system.h"
#include <immintrin.h>
#include "cpu/geo_context/geo_context.h"
void ParticlePageCPU::update(float deltaTime, GeoContextCPU &context){
    const int increment = 16; // 16 floats = 512 bits

    // integrate
    u32 i = 0;
    u32 size = nbParticles - (nbParticles % 16);
    const __m512 delta = _mm512_set1_ps(deltaTime);

    const __m512 gravityExpanded[3] = { // expand as virtual stream
        _mm512_set_ps(  gravity[0], gravity[2], gravity[1], gravity[0],
                        gravity[2], gravity[1], gravity[0], gravity[2],
                        gravity[1], gravity[0], gravity[2], gravity[1],
                        gravity[0], gravity[2], gravity[1], gravity[0]
                    ),
        _mm512_set_ps(  gravity[1], gravity[0], gravity[2], gravity[1],
                        gravity[0], gravity[2], gravity[1], gravity[0],
                        gravity[2], gravity[1], gravity[0], gravity[2],
                        gravity[1], gravity[0], gravity[2], gravity[1]
                    ),
        _mm512_set_ps(  gravity[2], gravity[1], gravity[0], gravity[2],
                        gravity[1], gravity[0], gravity[2], gravity[1],
                        gravity[0], gravity[2], gravity[1], gravity[0],
                        gravity[2], gravity[1], gravity[0], gravity[2]
                    )
    };
    const __m512 scaledGrav[3] = { 
        _mm512_mul_ps(gravityExpanded[0], delta),
        _mm512_mul_ps(gravityExpanded[1], delta),
        _mm512_mul_ps(gravityExpanded[2], delta)
    };
    while (i*3 <= size){
        // TODO mask the particles that have a lifetime < 0 or > maxLifetime so we don't integrate them
        float *ptrPos = reinterpret_cast<float *>(position) + i*3 ;
        float *ptrVel =  reinterpret_cast<float *>(velocity) + i*3 ;
        // update velocities and positions
        // We have to do 3 at the same time since it's vec3s, so the scalar attributes stay aligned with the flow
        for (u32 offsetIdx = 0; offsetIdx < 3; offsetIdx++){
            const u32 offset = offsetIdx * 16;
            const __m512 pos = _mm512_load_ps(ptrPos + offset);
            const __m512 vel = _mm512_load_ps(ptrVel + offset);

            const __m512 updatedVel = _mm512_add_ps(vel, scaledGrav[offsetIdx]);

            const __m512 scaledVel = _mm512_mul_ps(vel, delta);

            const __m512 updatedPos = _mm512_add_ps(pos, scaledVel);
            _mm512_stream_ps(ptrVel + offset, updatedVel);
            _mm512_stream_ps(ptrPos + offset, updatedPos);
        }
        // update lifetime
        float *ptrLifetime = reinterpret_cast<float *>(lifetime) + i;
        const __m512 lifetimes = _mm512_load_ps(ptrLifetime);
        _mm512_stream_ps(ptrLifetime, _mm512_add_ps(lifetimes, delta));

        i += increment;
    }
    for (; i < nbParticles; i++){ // end of unroll
        velocity[i] = velocity[i] + gravity * deltaTime;
        position[i] = position[i] + velocity[i] * deltaTime;
        lifetime[i] += deltaTime;
    }

    // collide
    // This is not simded for now. Do it if perfs are not enough !
    i = 0;
    finished = 0;
    for (; i < nbParticles; i++){
        float px = position[i][0];
        float py = position[i][1];
        float pz = position[i][2];

        if (lifetime[i] >= 0.0 && lifetime[i] < maxLifeTime &&
            (px >= 0.0f && px < 1.0f) &&
            (pz >= 0.0f && pz < 1.0f))
        {
            uvec2 pos = uvec2(px * IMGSIZE, pz * IMGSIZE);
            u8 mat = 0;
            float h = context.totalHeight(pos, &mat) * heightFactor;
            if (py >= -0.01 && py <= h){
                position[i][1] = h;
                float dh_dx = (context.totalHeight(pos) - context.totalHeight(pos + uvec2(1, 0)));
                float dh_dy = (context.totalHeight(pos) - context.totalHeight(pos + uvec2(0, 1)));

                // rebound 
                vec3 gx = vec3(1.0 / IMGSIZE, dh_dx, 0.0);
                vec3 gy = vec3(0.0, dh_dy, 1.0 / IMGSIZE);
                vec3 n = vec3::cross(gx, gy).normalized();
                velocity[i] = velocity[i].reflect(n) * 0.2;

                // impact on context
                float placed = water[i] / 2.0;
                context.maps[to_underlying(MAP_LAYERS::WATER)](pos) += placed;
                water[i] -= placed;

                placed = dust[i] / 2.0;
                context.maps[to_underlying(MAP_LAYERS::SAND)](pos) += placed;
                water[i] -= placed;
                if (mat == to_underlying(MAP_LAYERS::WATER)){
                    float displaced = context.maps[mat](pos) / 100.0;
                    water[i] += displaced;
                    context.maps[mat](pos) -= displaced;
                } else if (mat == to_underlying(MAP_LAYERS::SAND)){
                    float displaced = context.maps[mat](pos) / 100.0;
                    dust[i] += displaced;
                    context.maps[mat](pos) -= displaced;
                }
            }
        } else if (lifetime[i] >= maxLifeTime) {
            finished++;

        }
    }
}

void ParticlePageCPU::addParticle(const vec3 &position, const vec3 &velocity, float startingLifetime, float baseWater, float baseDust)
{
    this->position[nbParticles] = position;
    this->velocity[nbParticles] = velocity;
    dust[nbParticles] = baseDust;
    water[nbParticles] = baseWater;
    this->lifetime[nbParticles] = 0.0f;
    nbParticles++;
}
