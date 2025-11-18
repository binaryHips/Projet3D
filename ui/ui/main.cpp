#include "mainwindow.h"
#include "mapitem.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MapItem * item = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(&item->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    MapItem * item_2 = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(&item_2->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    MapItem * item_3 = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(&item_3->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    MapItem * item_4 = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(&item_4->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    MapItem * item_5 = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(&item_5->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    w.ui->maps_layout->addLayout(&item->layout);
    w.ui->maps_layout->addLayout(&item_2->layout);
    w.ui->maps_layout->addLayout(&item_3->layout);
    w.ui->maps_layout->addLayout(&item_4->layout);
    w.ui->maps_layout->addLayout(&item_5->layout);


    QObject::connect(w.ui->test_btn , SIGNAL(clicked()), &w , SLOT(returnClicked()));

    w.ui->maps_layout->addStretch();


    w.show();
    return a.exec();
}
