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

    // prepare per-layer overlay storage
    layerOverlays.resize(to_underlying(MAP_LAYERS::MAX_));

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
    //QObject::connect(ui->actionLoad_Heightmap , &QAction::triggered , this , &MainWindow::openFileSearch);
    QObject::connect(ui->actionBedrock_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::BEDROCK);});
    QObject::connect(ui->actionStone_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::STONE);});
    QObject::connect(ui->actionSand_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::SAND);});
    QObject::connect(ui->actionWater_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::WATER);});

    QObject::connect(this , &MainWindow::updateGLSignal , ui->widget , &GLWidget::updateGLSlot);

    // connect backend signals (new-style connection forwards the QString filename)
    QObject::connect(backend, &Backend::loadMapSignal, this, &MainWindow::setHeightMap);

    // Drawing page (page 2)

    ui->draw_settings_layout->addStretch();

    QObject::connect(backend, &Backend::updateMapSignal, this, &MainWindow::updateMap);

}

void MainWindow::mapClicked(QPixmap pixmap , MAP_LAYERS layer)
{
    // TODO : Fix the height things

    // save current overlay for current layer before switching
    MAP_LAYERS current = ui->widget_2->layer;
    QPixmap currentOverlay = ui->widget_2->getOverlayPixmap();
    if (!currentOverlay.isNull()) {
        layerOverlays[to_underlying(current)] = currentOverlay;
    }

    // set new background and layer
    pixmap = pixmap.scaled(ui->widget_2->size() , Qt::KeepAspectRatio);
    ui->widget_2->setBackgroundPixmap(pixmap);
    ui->widget_2->layer = layer;

    // restore overlay for this layer if we have one, otherwise clear
    QPixmap saved = layerOverlays[to_underlying(layer)];
    if (!saved.isNull()) ui->widget_2->setOverlayPixmap(saved);
    else ui->widget_2->clearOverlay();

    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::openFileSearchHeightmap(MAP_LAYERS layer)
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
    backend->loadHeightmap(fileName, layer); //FIXME reaaally temporary. Let the user choose in the end
    backend->saveImageFromMap(layer);
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

void MainWindow::setHeightMap(QString filename , MAP_LAYERS layer)
{
    MapItem *item = new MapItem(filename, this);
    item->m_layer = layer;
    if (item->map_image) item->map_image->layer = layer;
    QObject::connect(item->map_image, &ClickableLabel::clicked, this, &MainWindow::mapClicked);

    ui->maps_layout->addWidget(item);
    ui->maps_layout->addStretch();

    emit updateGLSignal();

}


// FIXME je crois que j'ai fait n'importe quoi en vrai de vrai ??
void MainWindow::updateMap(QPixmap map , MAP_LAYERS layer)
{
    qDebug() << "HELLO" ;
    for(int i = 0 ; i < ui->maps_layout->count() ; i++)
    {
        //https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof
        if(MapItem *item = dynamic_cast<MapItem*>(ui->maps_layout->itemAt(i)->widget())) {
            qDebug() << "ok we have items";
            if(item->m_layer == layer){
                qDebug() << "Found map";
                item->updateMap(map);
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
    // persist overlay for this layer
    MAP_LAYERS layer = ui->widget_2->layer;
    QPixmap overlay = ui->widget_2->getOverlayPixmap();
    if (!overlay.isNull()) layerOverlays[to_underlying(layer)] = overlay;

    backend->setHeightmap(ui->widget_2->getImage() , layer); // GET ACTUAL MAP
    // request GL widget to update its heightmap from backend context
    emit updateGLSignal(); // TODO : eventually make this only in backend.

}


// TODO : Figure out a better way to do this
// TODO : Remove the checked
void MainWindow::on_blackButton_clicked(bool checked)
{
    qDebug() << "black btn : " << checked;
    ui->blackButton->setStyleSheet("QPushButton{border: 2px solid #00008b; border-radius: 4px; background-color: rgba(0,0,0,255);}");

    // lowkey you need to do ts for all of them if we add more colors
    ui->whiteButton->setStyleSheet("QPushButton{border:none; background-color: rgba(255,255,255,255);}");
    ui->widget_2->setPenColor(QColor(0,0,0));

}

void MainWindow::on_whiteButton_clicked(bool checked)
{
    ui->whiteButton->setStyleSheet("QPushButton{border: 2px solid #00008b; border-radius: 4px; background-color: rgba(255,255,255,255);}");
    ui->blackButton->setStyleSheet("QPushButton{border:none; background-color: rgba(0,0,0,255);}");
    ui->widget_2->setPenColor(QColor(255,255,255));
}
