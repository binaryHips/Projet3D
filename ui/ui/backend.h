#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include <QWidget>
#include "includes_all_cpu.h"


class Backend : public QWidget
{
    Q_OBJECT
public :
    Backend(QWidget *parent);
    ~Backend();
    GeoContextCPU context = GeoContextCPU::createGeoContext();
    MapCPU loadHeightmap(QString filename, float scale = 1.0f);
    bool simulating;

signals :
        void loadMapSignal(QString filename);



};

#endif // BACKEND_H
