#include "mapitem.h"
#include <iostream>
#include <QResizeEvent>

#include "backend.h"
#include "mainwindow.h"

MapItem::MapItem(QWidget *parent)
    : QWidget{parent}
{
    map_image = new ClickableLabel(this);
    map_image->setSizePolicy(QSizePolicy::Minimum,  QSizePolicy::Minimum);
}

MapItem::MapItem(QPixmap image, MAP_LAYERS layer, QWidget *parent)
    : QWidget{parent}
{
    map = image;
    m_layer = layer;
    map_image = new ClickableLabel(this);
    layout = new QVBoxLayout(this);

    map_image->setSizePolicy(QSizePolicy::Minimum,  QSizePolicy::Minimum);

    int h = map_image->height() ;

    map_image->setPixmap(map.scaled(h,h, Qt::IgnoreAspectRatio));

    GeoContextCPU context = static_cast<MainWindow*>(window())->backend->context;
    QLabel* layerName = new QLabel(QString::fromStdString(context.maps[to_underlying(m_layer)].name));

    layout->addWidget(layerName);
    layout->addWidget(map_image);
    layout->setStretch(1,5);

}

MapItem::MapItem(QPixmap image, FEATURE_LAYERS layer, QWidget *parent)
    : QWidget{parent}
{
    map = image;
    m_feat_layer = layer;
    map_image = new ClickableLabel(this);
    layout = new QVBoxLayout(this);
    isMap = false;

    map_image->setSizePolicy(QSizePolicy::Minimum,  QSizePolicy::Minimum);

    int h = map_image->height() ;

    map_image->setPixmap(map.scaled(h,h, Qt::IgnoreAspectRatio));

    GeoContextCPU context = static_cast<MainWindow*>(window())->backend->context;
    QLabel* layerName = new QLabel(QString::fromStdString(context.featureMaps[to_underlying(m_feat_layer)].name));

    layout->addWidget(layerName);
    layout->addWidget(map_image);
    layout->setStretch(1,5);

}

void MapItem::updateMap(QPixmap im) // Or pixmap or string idk we'll see
{
    map = im;
    // Protect against map_image having zero height (not yet laid out).
    int h = map_image->height();
    const int fallback = 64;
    if (h <= 0) h = fallback;

    if (!map.isNull()) {
        map_image->setPixmap(map.scaled(h, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        map_image->clear();
    }
}

void MapItem::resizeEvent(QResizeEvent* event)
{
    // QWidget::resizeEvent(event);

    int h = map_image->height() ;
    QPixmap px = map.scaled(h,h,Qt::IgnoreAspectRatio);
    map_image->setPixmap(px);
}

MapItem::~MapItem()
{
}
