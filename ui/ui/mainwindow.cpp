#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "backend.h"
#include "cameracontroller.h"
#include <QFileDialog>
#include <QDir>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MapCPU heightmap = Backend::loadHeightmap(":/test.png");
    context.addMap(std::move(heightmap));

    ui->setupUi(this);

    // connect camera mode radio buttons to GLWidget
    QObject::connect(ui->radioButton_2, &QRadioButton::toggled, ui->widget, [w = ui->widget](bool checked){ if (checked) w->setControlType(ORBITAL); });
    QObject::connect(ui->radioButton, &QRadioButton::toggled, ui->widget, [w = ui->widget](bool checked){ if (checked) w->setControlType(FPS); });
    ui->radioButton_2->setChecked(true);

    // addStretch() to make them hug top
    ui->gl_settings_layout->addStretch();

    // Load our plane
    int nbDiv = 100;
    Mesh *plane= new Mesh();
    *plane = Mesh::gen_tesselatedSquare(nbDiv,nbDiv,1,1);
    ui->widget->addMesh(plane);

    // connect load mesh -> TODO figure out how to do it from editor
    QObject::connect(ui->actionLoad_Heightmap , &QAction::triggered , this , &MainWindow::openFileSearch);
}

void MainWindow::mapClicked()
{
    std::cout << "help" << std::endl;
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::returnClicked()
{
    std::cout << "help" << std::endl;
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void MainWindow::openFileSearch()
{
    qDebug() << "Called " ;
    // QString fileName = QFileDialog::getOpenFileName(this,
    //                                                 "Open Image",
    //                                                 QDir::homePath(),
    //                                                 "Image Files (*.png *.jpg *.jpeg *.bmp *.tga);;All Files (*)");
    
    QString fileName;
    
    QFileDialog dlg(this,"Open Image", QDir::homePath());
    dlg.setNameFilter("Image Files (*.png *.jpg *.jpeg);;All Files()");
    dlg.setOption(QFileDialog::DontUseNativeDialog);
    if(dlg.exec() == QDialog::Accepted)
    {
        qDebug() << "huh";
        fileName = dlg.selectedFiles().first();
    }

    qDebug() << "Opened" ;

    if (!fileName.isEmpty())
    {
        qDebug() << "found" ;
        Backend::loadHeightmap(fileName);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_subdiv_slider_valueChanged(int value)
{
    qDebug() << "hello" ;
    Mesh *plane= new Mesh();
    *plane = Mesh::gen_tesselatedSquare(value,value,1,1);
    ui->widget->setMesh(plane,0);
}

