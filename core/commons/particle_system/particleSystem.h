#pragma once
#include "maths/maths.h"
#include <vector>

class ParticlePageBase__{
protected:
    enum {
        PAGE_SIZE = 256
    };
    u32 nbParticles;

    void update(float deltaTime);
};

class ParticleSystemBase__{
protected:
    void update(float deltaTime);

    void spawn(u32 n);


};