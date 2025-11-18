#include "mainwindow.h"
#include "mapitem.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MapItem * item = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    MapItem * item_2 = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");

    w.ui->maps_layout->addLayout(&item->layout);
    w.ui->maps_layout->addLayout(&item_2->layout);

    w.ui->maps_layout->addStretch();

    w.show();
    return a.exec();
}
