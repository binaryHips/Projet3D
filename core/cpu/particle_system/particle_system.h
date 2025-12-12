#pragma once
#include "commons/particle_system/particle_system.h"
#include <functional>

class GeoContextCPU;
class ParticlePageCPU : public ParticlePageBase__{
public:
    // align to 512 bits
    alignas(64) vec3 position[PAGE_SIZE];

    alignas(64) vec3 velocity[PAGE_SIZE];

    // Maybe float is too big? could use u8 ? idk

    alignas(64) float water[PAGE_SIZE];
    alignas(64) float dust[PAGE_SIZE];
    
    alignas(64) float lifetime[PAGE_SIZE];

    float maxLifeTime = 5.0;
    int finished = 0; // number of particles already finished

    // The direction is used to define the start direction,so the way the particle come frm and go toward.
    vec3 direction = vec3(1.0, 0.0, 0.0);
    vec3 gravity = vec3(0.0, 1.0, 0.0);

    void update(float deltaTime, GeoContextCPU &context);
    void addParticle(const vec3 &position, const vec3 &velocity, float startingLifetime, float baseWater, float baseDust);
};

class ParticleSystemCPU : public ParticleSystemBase__{
public:
    std::vector<ParticlePageCPU> pages;
    GeoContextCPU *context;

    ParticleSystemCPU(GeoContextCPU *context)
        : context(context)
        {}
    
    ParticleSystemCPU() = default;

    void update(float deltaTime){
        for(auto page = pages.begin(); page != pages.end(); )
        {   
            if(page->finished >= ParticlePageCPU::PAGE_SIZE){
                page = pages.erase(page);
            }
            else {
                page->update(deltaTime, *context);
                page++;
            }
        }
    }
    void spawn(u32 nPages, float maxLifetimes, float spawnDuration, const vec3 &directionVector_p, const vec3& forceVector, float baseWater, float baseDust){
        vec3 directionVector = directionVector_p.normalized();
        while (nPages > 0){
            ParticlePageCPU& page = pages.emplace_back();

            page.direction = directionVector;
            page.gravity = forceVector;
            page.maxLifeTime = maxLifetimes;

            for (u32 i = 0; i < ParticlePageCPU::PAGE_SIZE; ++i){
                vec3 pos = vec3(rand()/float(RAND_MAX), rand()/float(RAND_MAX), rand()/float(RAND_MAX));
                pos += -directionVector * 2.0;
                float startingLifetime = 0.0; //- i * (1.0 / ParticlePageCPU::PAGE_SIZE) * spawnDuration; // this'd smoothen the spawn but requires lifetimes < 0 to not integrate (TODO in the simd code)
                page.addParticle(pos, directionVector_p, startingLifetime, baseWater, baseDust);
            }
            nPages--;
        }
    }
};
