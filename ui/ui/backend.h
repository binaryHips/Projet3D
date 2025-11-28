#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include "includes_all_cpu.h"


struct Backend
{
    Backend();

    GeoContextCPU context = GeoContextCPU::createGeoContext();

    MapCPU loadHeightmap(QString filename);



};

#endif // BACKEND_H
