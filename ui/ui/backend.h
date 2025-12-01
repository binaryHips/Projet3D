#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include <QWidget>
#include <QOpenGLExtraFunctions>

#include "includes_all_cpu.h"


class Backend : public QWidget
{
    Q_OBJECT
public :
    Backend(QWidget *parent);
    ~Backend();
    GeoContextCPU context = GeoContextCPU::createGeoContext();
    MapCPU loadHeightmap(QString filename, MAP_LAYERS layer, float scale = 1.0f);
    static void drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem);
    bool simulating;

signals :
        void loadMapSignal(QString filename);



};

#endif // BACKEND_H
