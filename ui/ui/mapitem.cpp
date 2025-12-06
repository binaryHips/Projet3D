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

MapItem::MapItem(QString image_path, QWidget *parent)
    : QWidget{parent}
{
    map_image = new ClickableLabel(this);
    layout = new QVBoxLayout(this);
    map_image->setSizePolicy(QSizePolicy::Minimum,  QSizePolicy::Minimum);

    // Add the image
    map = QPixmap(image_path);

    // map_image->setText("HELLO");
    // resize(256,256);
    int h = map_image->height() ;
    map_image->setPixmap(map.scaled(h,h, Qt::IgnoreAspectRatio));
    map_image->layer = m_layer;

    layout->addWidget(map_image);
}

MapItem::MapItem(QPixmap image, QWidget *parent)
    : QWidget{parent}
{
    map = image;
    map_image = new ClickableLabel(this);
    layout = new QVBoxLayout(this);
    map_image->setSizePolicy(QSizePolicy::Minimum,  QSizePolicy::Minimum);

    int h = map_image->height() ;

    map_image->setPixmap(map.scaled(h,h, Qt::IgnoreAspectRatio));

    layout->addWidget(map_image);

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
    std::cout << "height : " << h << std::endl;

    QPixmap px = map.scaled(h,h,Qt::IgnoreAspectRatio);
    map_image->setPixmap(px);
}

MapItem::~MapItem()
{
}
