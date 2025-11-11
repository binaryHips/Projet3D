#pragma once
#include "commons/particle_system/particle_system.h"


class ParticlePageCPU : public ParticlePageBase__{

    vec3 positions[PAGE_SIZE];
    vec3 velocity[PAGE_SIZE];

    

    void update(float deltaTime);
};

class ParticleSystemCPU : public ParticleSystemBase__{

    std::vector<ParticlePageCPU> pages;

    void update(float deltaTime);

    void spawn(u32 n);

};