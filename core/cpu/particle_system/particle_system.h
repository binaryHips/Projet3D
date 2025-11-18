#pragma once
#include "commons/particle_system/particle_system.h"


class ParticlePageCPU : public ParticlePageBase__{
    // align to 512 bits
    #pragma align(64) vec3 position[PAGE_SIZE];
    #pragma align(64) vec3 velocity[PAGE_SIZE];

    // Maybe float is too big? could use u8 ? idk
    #pragma align(64) float water[PAGE_SIZE];
    #pragma align(64) float dust[PAGE_SIZE];
    
    #pragma align(64) float lifetime[PAGE_SIZE];

    void update(float deltaTime);
};

class ParticleSystemCPU : public ParticleSystemBase__{

    std::vector<ParticlePageCPU> pages;

    void update(float deltaTime);

    void spawn(u32 n);

};