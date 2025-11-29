#include "mainwindow.h"
#include "mapitem.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "backend.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MapItem * item = new MapItem(":/test.png");
    QObject::connect(item->map_image , SIGNAL(clicked()) , &w , SLOT(mapClicked()));

    w.ui->maps_layout->addWidget(item);

    Mesh *plane= new Mesh();
    // *plane = Mesh::load_mesh_off(":/suzanne.off");

    *plane = Mesh::gen_tesselatedSquare(100,100,1,1);

    w.ui->widget->addMesh(plane);

    QObject::connect(w.ui->test_btn , SIGNAL(clicked()), &w , SLOT(returnClicked()));

    w.ui->maps_layout->addStretch();

    // Load the fuckin uhh heightmap thing
    // Prolly move it somewhere else lmao
    QAction *loadHeightmapAction = w.ui->menuFile->addAction("Load Heightmap");
    QObject::connect(loadHeightmapAction, &QAction::triggered, &w, &MainWindow::openFileSearch);

    w.show();
    return a.exec();
}

void openFileSearch(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    "Open Image",
                                                    QDir::homePath(),
                                                    "Image Files (*.png *.jpg *.jpeg *.bmp *.tga);;All Files (*)");
    if (!fileName.isEmpty())
    {
        Backend::loadHeightmap(fileName);
    }
}
