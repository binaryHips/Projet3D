#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "backend.h"
#include "mapitem.h"
#include "clickablelabel.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backend(new Backend(this))
{
    ui->setupUi(this);

    // prepare per-layer overlay storage

    // 3D Page (page 1)

    // connect camera mode radio buttons to GLWidget
    QObject::connect(ui->radioButton_2, &QRadioButton::toggled, ui->widget, [w = ui->widget](bool checked){ if (checked) w->setControlType(ORBITAL); });
    QObject::connect(ui->radioButton, &QRadioButton::toggled, ui->widget, [w = ui->widget](bool checked){ if (checked) w->setControlType(FPS); });
    ui->radioButton_2->setChecked(true);

    ui->gl_settings_layout->addStretch();

    // Load our plane
    int nbDiv = 100;
    Mesh *plane= new Mesh();
    *plane = Mesh::gen_tesselatedSquare(nbDiv,nbDiv,1,1);
    ui->widget->addMesh(plane);

    // connect load mesh
    QObject::connect(ui->actionBedrock_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::BEDROCK);});
    QObject::connect(ui->actionStone_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::STONE);});
    QObject::connect(ui->actionSand_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::SAND);});
    QObject::connect(ui->actionWater_layer , &QAction::triggered , this , [this]{openFileSearchHeightmap(MAP_LAYERS::WATER);});

    loadDefaultMaps();

    // Drawing page (page 2)

    ui->draw_settings_layout->addStretch();

    // TODO : See if making these the same signal is smart
    QObject::connect(backend, &Backend::loadMapSignal, this, &MainWindow::updateMap);
    QObject::connect(backend, &Backend::updateMapSignal, this, &MainWindow::updateMap);


    // Eraser button (ugly)
    QPixmap pixmap(":/ressources/icons/eraser.png");
    pixmap = pixmap.scaled(ui->eraserBtn->height(), ui->eraserBtn->height() , Qt::KeepAspectRatio);
    QIcon ButtonIcon(pixmap);
    ui->eraserBtn->setIcon(ButtonIcon);
    ui->eraserBtn->setIconSize(pixmap.rect().size());

    // init btn
    int initGrey = ui->greyscaleSlider->value();
    QString style = QString("QPushButton { background-color: rgb(%1, %1, %1); border: 2px solid #555; border-radius: 4px; }")
                    .arg(initGrey);
    ui->colorSelectBtn->setStyleSheet(style);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDefaultMaps()
{
    QVector<MAP_LAYERS> maps = {MAP_LAYERS::BEDROCK , MAP_LAYERS::STONE , MAP_LAYERS::SAND , MAP_LAYERS::WATER};

    for(MAP_LAYERS layer : maps)
    {
        QPixmap map = backend->saveImageFromMap(layer);
        MapItem *item = new MapItem(map, layer, this);
        item->m_layer = layer;
        if (item->map_image) {
            item->map_image->layer = layer;
            QObject::connect(item->map_image, &ClickableLabel::clicked, this, &MainWindow::mapClicked);
        }
        ui->maps_layout->addWidget(item);
        // ui->maps_layout->addLayout(item->layout);

    }

    ui->maps_layout->addStretch();

}

void MainWindow::openFileSearchHeightmap(MAP_LAYERS layer)
{
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
        backend->loadHeightmap(fileName, layer);
        backend->saveImageFromMap(layer);
    }
}

void MainWindow::mapClicked(QPixmap pixmap , MAP_LAYERS layer)
{


    // set new background and layer
    pixmap = pixmap.scaled(ui->widget_2->size() , Qt::KeepAspectRatio);
    ui->widget_2->setBackgroundPixmap(pixmap);
    ui->widget_2->layer = layer;

    // restore overlay for this layer if we have one, otherwise clear
    ui->widget_2->clearOverlay();

    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::updateMap(QPixmap map , MAP_LAYERS layer)
{
    for(int i = 0 ; i < ui->maps_layout->count() ; i++)
    {
        //https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof
        if(MapItem *item = dynamic_cast<MapItem*>(ui->maps_layout->itemAt(i)->widget())) {
            if(item->m_layer == layer) item->updateMap(map);
        }
    }
    ui->widget->updateGLSlot();
}

void MainWindow::on_subdiv_slider_valueChanged(int value)
{
    Mesh *plane= new Mesh();
    *plane = Mesh::gen_tesselatedSquare(value,value,1,1);
    ui->widget->setMesh(plane,0);
    ui->subdivval_label->setText(QString::number(value));
}

void MainWindow::on_simspeedslider_valueChanged(int value)
{
    float val = value / 100.0f;
    ui->simspeedval_label->setText(QString::number(val , 'f' , 2));
    ui->widget->simSpeed = val;
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

void MainWindow::on_pensizeSlider_valueChanged(int value)
{
    ui->pensize_label->setText(QString::number(value) + " px");
    ui->widget_2->setPenWidth(value);
}

void MainWindow::on_opacityValSLider_valueChanged(int value)
{
    ui->opacityValLabel->setText(QString::number(value));
    ui->widget_2->setPenOpacity(value);
}

void MainWindow::on_greyscaleSlider_valueChanged(int value)
{
    // update button color to the slider's value
    QString style = QString("QPushButton { background-color: rgb(%1, %1, %1); border: 2px solid #555; border-radius: 4px; }").arg(value);
    ui->colorSelectBtn->setStyleSheet(style);
}


void MainWindow::on_eraserBtn_clicked()
{
    // Hightlight this and unhighlight the other (same fot other btn)
    ui->eraserBtn->setStyleSheet("QPushButton{border: 2px solid #B700FF; border-radius: 4px;}");
    ui->colorSelectBtn->setStyleSheet(ui->colorSelectBtn->styleSheet().replace("border: 2px solid #B700FF", "border: 2px solid #555"));
    ui->widget_2->setEraser(true);
}

void MainWindow::on_colorSelectBtn_clicked()
{

    int value = ui->greyscaleSlider->value();
    
    ui->widget_2->setPenColor(QColor(value, value, value));
    ui->widget_2->setEraser(false);
    
    QString style = QString("QPushButton { background-color: rgb(%1, %1, %1); border: 2px solid #B700FF; border-radius: 4px; }").arg(value);
    ui->colorSelectBtn->setStyleSheet(style);
    ui->eraserBtn->setStyleSheet("QPushButton{border: none;}");
}


void MainWindow::on_resetDrawingBtn_clicked()
{
    ui->widget_2->clearOverlay();
}

// FIXME : Either fix saved resolution, or don't save overlay, or call only original image idk.
void MainWindow::on_confirmMapBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
    MAP_LAYERS layer = ui->widget_2->layer;
    backend->setHeightmap(ui->widget_2->getImage() , layer);
    ui->widget->updateGLSlot();

}

void MainWindow::on_numParticlesSlider_valueChanged(int value)
{
    m_particles = value;
    ui->numParticlesLabel->setText(QString::number(value));
}

void MainWindow::on_spawnParticlesBtn_clicked()
{
    ui->widget->spawnParticles(m_particles);
}


