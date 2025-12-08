#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "backend.h"
#include "mapitem.h"
#include "clickablelabel.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

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

    // QObject::connect(this , &MainWindow::updateGLSignal , ui->widget , &GLWidget::updateGLSlot);

    loadDefaultMaps();

    // Drawing page (page 2)

    ui->draw_settings_layout->addStretch();

    // TODO : See if making these the same signal is smart
    QObject::connect(backend, &Backend::loadMapSignal, this, &MainWindow::updateMap);
    QObject::connect(backend, &Backend::updateMapSignal, this, &MainWindow::updateMap);

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
        MapItem *item = new MapItem(map, this);
        item->m_layer = layer;
        if (item->map_image) {
            item->map_image->layer = layer;
            QObject::connect(item->map_image, &ClickableLabel::clicked, this, &MainWindow::mapClicked);
        }
        ui->maps_layout->addWidget(item);

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
    qDebug() << "Update maps" ;
    for(int i = 0 ; i < ui->maps_layout->count() ; i++)
    {
        //https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof
        if(MapItem *item = dynamic_cast<MapItem*>(ui->maps_layout->itemAt(i)->widget())) {
            if(item->m_layer == layer) item->updateMap(map);
        }
    }
}

void MainWindow::on_subdiv_slider_valueChanged(int value)
{
    qDebug() << "hello" ;
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

void MainWindow::on_blackButton_clicked()
{
    ui->blackButton->setStyleSheet("QPushButton{border: 2px solid #00008b; border-radius: 4px; background-color: rgba(0,0,0,255);}");

    // lowkey you need to do ts for all of them if we add more colors
    ui->whiteButton->setStyleSheet("QPushButton{border:none; background-color: rgba(255,255,255,255);}");
    ui->widget_2->setPenColor(QColor(0,0,0));

}

void MainWindow::on_whiteButton_clicked()
{
    ui->whiteButton->setStyleSheet("QPushButton{border: 2px solid #00008b; border-radius: 4px; background-color: rgba(255,255,255,255);}");
    ui->blackButton->setStyleSheet("QPushButton{border:none; background-color: rgba(0,0,0,255);}");
    ui->widget_2->setPenColor(QColor(255,255,255));
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
    // ui->widget->updateGLSlot();

}
