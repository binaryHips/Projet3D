#include "backend.h"

#include <QString>
#include <QImage>
#include <QDebug>

Backend::Backend() = default;

Backend::~Backend() = default;

MapCPU Backend::loadHeightmap(QString filename, float scale)
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

    emit loadMapSignal(filename);
    context.addMap(std::move(res));
    return res;

}
