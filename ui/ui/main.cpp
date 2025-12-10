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

    // Style sheet : https://qss-stock.devsecstudio.com/templates.php
    QFile styleFile( ":/Takezo.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    a.setStyleSheet( style );


    // w.ui->maps_layout->addStretch();
    w.show();
    return a.exec();
}
