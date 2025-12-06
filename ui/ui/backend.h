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
    MapCPU setHeightmap(QPixmap pixmap, MAP_LAYERS layer, float scale = 1.0f);
    static void drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem);

    // for debug mostly
    QPixmap saveImageFromMap(MAP_LAYERS layer);
    bool simulating;

signals :
    void loadMapSignal(QString filename , MAP_LAYERS layer);
    void updateMapSignal(QPixmap pixmap, MAP_LAYERS layer);

};

#endif // BACKEND_H
