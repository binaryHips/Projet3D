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

//            qDebug() << "color mean : " << mean;

            res(x,y) = mean * scale;
        }
    }
    context.maps[to_underlying(layer)] = std::move(res); //FIXME temporary i think
    emit loadMapSignal(filename);
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

            //            qDebug() << "color mean : " << mean;

            res(x,y) = mean * scale;
        }
    }
    context.maps[to_underlying(layer)] = std::move(res); //FIXME temporary i think
    return res;
}

void Backend::drawParticles(QOpenGLExtraFunctions *gl_funcs, const ParticleSystemCPU &particleSystem)
{
    for(auto &page : particleSystem.pages){
        GLuint buf;
        gl_funcs->glGenBuffers(1, &buf);
        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, buf);
        gl_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * page.nbParticles , page.position ,  GL_STATIC_DRAW);
        gl_funcs->glDrawArrays(GL_POINTS, 0 , page.nbParticles);

        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER , 0);
        gl_funcs->glDeleteBuffers(1, &buf);
    }
}

// TODO : function to add colors to maps but also like combine the maps to get the final main map type shit

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
