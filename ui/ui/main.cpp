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

    QObject::connect(w.ui->test_btn , SIGNAL(clicked()), &w , SLOT(returnClicked()));

    w.ui->maps_layout->addStretch();
    w.show();
    return a.exec();
}
