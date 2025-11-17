#include "mapitem.h"
#include <iostream>

MapItem::MapItem(QWidget *parent)
    : QWidget{parent}
{}

MapItem::MapItem(QString image_path, QWidget *parent)
    : QWidget{parent}
{
    // Add the image
    QPixmap *map = new QPixmap(image_path);

    // TODO : Make it adapt to the size of the ting
    int w = map_image.width()/2;
    int h = map_image.height()/2;

    map_image.setPixmap(map->scaled(w,h,Qt::KeepAspectRatio));

    layout.addWidget(&map_image);

}


MapItem::~MapItem(){
}
