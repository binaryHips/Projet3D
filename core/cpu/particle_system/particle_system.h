#pragma once
#include "commons/particle_system/particle_system.h"

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

    void update(float deltaTime, const GeoContextCPU &context);
    void addParticle(const vec3 &position, const vec3 &velocity);
};

class ParticleSystemCPU : public ParticleSystemBase__{
public:
    std::vector<ParticlePageCPU> pages;
    const GeoContextCPU *context;


    // The direction is used to define the wind direction,so the way the particle come frm and go toward.
    vec3 direction = vec3(1.0, 0.0, 0.0);

    ParticleSystemCPU(const GeoContextCPU *context)
        : context(context)
        {}
    
    ParticleSystemCPU() = default;

    void update(float deltaTime){
        for (ParticlePageCPU& page: pages){
            page.update(deltaTime, *context);
        }
    }
    void spawn(u32 n){
        std::cout << "HUHHH" << std::endl;
        while (n > 0){
            std::cout << "n : " << n << std::endl;
            if (pages.empty()){
                pages.emplace_back();
            }

            if(pages.back().nbParticles >= ParticlePageCPU::PAGE_SIZE)
            {
                pages.emplace_back();
            }

            ParticlePageCPU& lastPage = pages.back();
            const u32 remainingInLastPage = ParticlePageCPU::PAGE_SIZE - lastPage.nbParticles;

            // fill the page like crazy
            const u32 toFill = std::min(remainingInLastPage, n);

            for (u32 i = 0; i < toFill; ++i){
                lastPage.addParticle(vec3(0.0f), vec3(rand()/float(RAND_MAX), 2.0f, rand()/float(RAND_MAX)));
            }

            n -= toFill;

            // n-=1;

        }
    }
};
