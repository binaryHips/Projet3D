#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "backend.h"
#include "cameracontroller.h"
#include "mapitem.h"
#include "clickablelabel.h"
#include <QFileDialog>
#include <QDir>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // MapCPU heightmap = backend.loadHeightmap(":/test.png");
    // context.addMap(std::move(heightmap));


    backend = new Backend(this);

    ui->setupUi(this);

    // 3D Page (page 1)

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

    // connect backend signals (new-style connection forwards the QString filename)
    QObject::connect(backend, &Backend::loadMapSignal, this, &MainWindow::setHeightMap);

    // Drawing page (page 2)

    ui->draw_settings_layout->addStretch();

    QObject::connect(backend, &Backend::updateMapSignal, this, &MainWindow::updateMap);

}

void MainWindow::mapClicked(QPixmap pixmap)
{
    // TODO : Fix the height things

    pixmap = pixmap.scaled(ui->widget_2->size() , Qt::KeepAspectRatio);
    ui->widget_2->setBackgroundPixmap(pixmap);
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::openFileSearch()
{
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
        fileName = dlg.selectedFiles().first();
    }


    if (!fileName.isEmpty())
    {
        qDebug() << "File found";
    backend->loadHeightmap(fileName, MAP_LAYERS::SAND); //FIXME reaaally temporary. Let the user choose in the end
    // backend->saveImageFromMap(MAP_LAYERS::SAND);
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
    ui->subdivval_label->setText(QString::number(value));
}

void MainWindow::setHeightMap(QString filename)
{
    MapItem *item = new MapItem(filename, this);
    QObject::connect(item->map_image, &ClickableLabel::clicked, this, &MainWindow::mapClicked);

    ui->maps_layout->addWidget(item);
    ui->maps_layout->addStretch();
    // ui->widget->update();
}


// FIXME je crois que j'ai fait n'importe quoi en vrai de vrai ??
void MainWindow::updateMap(QPixmap map , MAP_LAYERS layer)
{
    qDebug() << "HELLO" ;
    for(int i = 0 ; i < ui->maps_layout->count() ; i++)
    {
        //https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof
        if(MapItem *item = dynamic_cast<MapItem*>(ui->maps_layout->itemAt(i)->widget())) {
            if(item->m_layer == layer){
                qDebug() << "Found map";
                item->updateMap(map);
            }
            else{
                return;
            }
        }
    }
}

void MainWindow::on_simulateBtn_clicked()
{
    backend->simulating = !backend->simulating;

    if(backend->simulating)
    {
        ui->simulateBtn->setText("Pause simulation");
    }
    else
    {
        ui->simulateBtn->setText("Launch simulation");
    }

}


void MainWindow::on_simspeedslider_valueChanged(int value)
{
    float val = value / 100.0f;
    ui->simspeedval_label->setText(QString::number(val , 'f' , 2));
    ui->widget->simSpeed = val;
}


void MainWindow::on_pensizeSlider_valueChanged(int value)
{
    ui->pensize_label->setText(QString::number(value) + " px");
    ui->widget_2->setPenWidth(value);
}


void MainWindow::on_resetDrawingBtn_clicked()
{
    ui->widget_2->clearOverlay();
}


void MainWindow::on_opacityValSLider_valueChanged(int value)
{
    ui->opacityValLabel->setText(QString::number(value));
    ui->widget_2->setPenOpacity(value);
}


void MainWindow::on_confirmMapBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
    backend->setHeightmap(ui->widget_2->getImage() , MAP_LAYERS::SAND);
}

