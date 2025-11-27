#include "mainwindow.h"
#include "mapitem.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MapItem * item = new MapItem("/home/drew/Desktop/3DProject/Pics/test.png");
    QObject::connect(item->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    w.ui->maps_layout->addWidget(item);


    Mesh *plane= new Mesh();
    // *plane = Mesh::load_mesh_off("/home/drew/Desktop/3DProject/Projet3D/ui/ui/ressources/suzanne.off");

    *plane = Mesh::gen_tesselatedSquare(100,100,1,1);

    w.ui->widget->addMesh(plane);

    QObject::connect(w.ui->test_btn , SIGNAL(clicked()), &w , SLOT(returnClicked()));

    w.ui->maps_layout->addStretch();


    w.show();
    return a.exec();
}
