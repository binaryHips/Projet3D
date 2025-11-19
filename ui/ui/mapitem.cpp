#include "mapitem.h"
#include <iostream>
#include <QResizeEvent>

MapItem::MapItem(QWidget *parent)
    : QWidget{parent}
{
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

    layout->addWidget(map_image);

}

void MapItem::resizeEvent(QResizeEvent* event)
{
    // QWidget::resizeEvent(event);

    int h = map_image->height() ;
    std::cout << "height : " << h << std::endl;

    QPixmap px = map.scaled(h,h,Qt::IgnoreAspectRatio);
    map_image->setPixmap(px);
}

void MapItem::clickedImage()
{
    std::cout << "hello" << std::endl;
}

MapItem::~MapItem()
{
    delete map_image;
}
