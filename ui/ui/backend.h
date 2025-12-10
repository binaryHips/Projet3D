#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include <QWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

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
    MapCPU setHeightmap(QPixmap pixmap, FEATURE_LAYERS layer, float scale = 1.0f);

    
    // Particle rendering
    void initParticleRenderer(QOpenGLExtraFunctions *gl_funcs);
    void drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem, const QMatrix4x4 &MVP);
    void cleanupParticleRenderer(QOpenGLExtraFunctions *gl_funcs);

    // for debug mostly
    QPixmap saveImageFromMap(MAP_LAYERS layer);
    QPixmap saveImageFromMap(FEATURE_LAYERS layer);
    bool simulating;

    // Particle rendering state
    QOpenGLShaderProgram *particleShader = nullptr;
    GLuint particleVAO = 0;
    GLuint particleVBO = 0;
    bool particleRendererInitialized = false;

signals :
    void loadMapSignal(QString filename , MAP_LAYERS layer);
    void updateMapSignal(QPixmap pixmap, MAP_LAYERS layer);
    void updateFeatureSignal(QPixmap pixmap, FEATURE_LAYERS layer);


};

#endif // BACKEND_H
