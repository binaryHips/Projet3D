#include "backend.h"

#include <QString>
#include <QImage>
#include <QDebug>
#include <QOpenGLExtraFunctions>

Backend::Backend(QWidget *parent)
    :QWidget(parent)
{
    simulating = false;
}

Backend::~Backend() = default;

MapCPU Backend::loadHeightmap(QString filename, MAP_LAYERS layer, float scale)
{
    QImage hmap = QImage();
    MapCPU res = MapCPU();

    if (!hmap.load(filename)) {
        qDebug() << "Heightmap " + filename << " was not found !";
        return res;
    }

    hmap = hmap.scaled(IMGSIZE , IMGSIZE);

    for (int y = 0; y < hmap.height(); y++) {
        for (int x = 0; x < hmap.width(); x++) {
            QRgb color = hmap.pixel(x,y);

            int r = color >> 0 & 0xFF;
            int g = color >> 8 & 0xFF;
            int b = color >> 16 & 0xFF;


            float mean = (r + g + b) / (3.0 * 255.0);

            res(x,y) = mean * scale;
        }
    }
    context.maps[to_underlying(layer)] = std::move(res); 
    emit loadMapSignal(filename , layer);
    return res;
}

MapCPU Backend::setHeightmap(QPixmap pixmap, MAP_LAYERS layer, float scale)
{
    QImage hmap = pixmap.toImage();
    MapCPU res = MapCPU();

    hmap = hmap.scaled(IMGSIZE , IMGSIZE);

    for (int y = 0; y < hmap.height(); y++) {
        for (int x = 0; x < hmap.width(); x++) {
            QRgb color = hmap.pixel(x,y);

            int r = color >> 0 & 0xFF;
            int g = color >> 8 & 0xFF;
            int b = color >> 16 & 0xFF;


            float mean = (r + g + b) / (3.0 * 255.0);

            res(x,y) = mean * scale;
        }
    }
    context.maps[to_underlying(layer)] = std::move(res);
    emit updateMapSignal(pixmap , layer);
    return res;
}

void Backend::initParticleRenderer(QOpenGLExtraFunctions *gl_funcs)
{
    if (particleRendererInitialized) return;

    // Create shader program
    particleShader = new QOpenGLShaderProgram();
    
    if (!particleShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ressources/shaders/particle_vshader.glsl")) {
        qDebug() << "Particle vertex shader failed:" << particleShader->log();
    }
    if (!particleShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ressources/shaders/particle_fshader.glsl")) {
        qDebug() << "Particle fragment shader failed:" << particleShader->log();
    }
    if (!particleShader->link()) {
        qDebug() << "Particle shader linking failed:" << particleShader->log();
    }

    // Create VAO and VBO
    gl_funcs->glGenVertexArrays(1, &particleVAO);
    gl_funcs->glGenBuffers(1, &particleVBO);

    gl_funcs->glBindVertexArray(particleVAO);
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

    // Setup vertex attribute for position (location 0, vec3)
    gl_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
    gl_funcs->glEnableVertexAttribArray(0);

    gl_funcs->glBindVertexArray(0);

    particleRendererInitialized = true;
    qDebug() << "Particle renderer initialized successfully";
}

void Backend::drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem, const QMatrix4x4 &MVP)
{
    if (!particleRendererInitialized) {
        initParticleRenderer(gl_funcs);
    }

    // Count total particles for debug
    int totalParticles = 0;
    for (const auto &page : particleSystem.pages) {
        totalParticles += page.nbParticles;
    }
    
    if (totalParticles == 0) {
        return; 
    }

    // Apparntly mandatory for the points to work
    gl_funcs->glEnable(GL_PROGRAM_POINT_SIZE);

    particleShader->bind();
    particleShader->setUniformValue("MVP", MVP);
    particleShader->setUniformValue("pointSize", 10.0f);  
    particleShader->setUniformValue("particleColor", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));  

    gl_funcs->glBindVertexArray(particleVAO);

    for (const auto &page : particleSystem.pages) {
        if (page.nbParticles == 0) continue;

        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
        gl_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * page.nbParticles, page.position, GL_STREAM_DRAW);

        gl_funcs->glDrawArrays(GL_POINTS, 0, page.nbParticles);
    }

    gl_funcs->glBindVertexArray(0);
    particleShader->release();

    gl_funcs->glDisable(GL_PROGRAM_POINT_SIZE);
}


QPixmap Backend::saveImageFromMap(MAP_LAYERS layer)
{
    const MapCPU &map = context.maps[to_underlying(layer)];

    QImage image(IMGSIZE, IMGSIZE, QImage::Format_ARGB32);
    for (int y = 0; y < IMGSIZE; ++y) {
        for (int x = 0; x < IMGSIZE; ++x) {

            float v = map(x, y);
            v = qBound(0.0f, v, 1.0f);

            int grey = static_cast<int>(v * 255.0f + 0.5f);
            image.setPixel(x, y, qRgb(grey, grey, grey));
        }
    }

    QPixmap pix = QPixmap::fromImage(image);

    // SAVE MAP FOR DEBUG PURPOSES !!
    // QString filename = QString("map_test.png");
    // pix.save(filename);

    return pix;
}
