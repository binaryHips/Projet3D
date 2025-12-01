#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include <QOpenGLExtraFunctions>

#include "includes_all_cpu.h"


struct Backend
{
    Backend();

//    GeoContextCPU context = GeoContextCPU::createGeoContext();

    static MapCPU loadHeightmap(QString filename, float scale = 1.0f);
    static void drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem);

};

#endif // BACKEND_H
