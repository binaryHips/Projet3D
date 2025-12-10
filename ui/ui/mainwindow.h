#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend.h"
#include "mapdrawwindow.h"
#include "scribblearea.h"
#include "glwidget.h"
#include "includes_all_cpu.h"
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    // Context that will handle all the backend, maybe switch to use Backend.h
    Backend *backend;

    // store per-layer overlay pixmaps so drawings aren't shared between layers
    std::vector<QPixmap> layerOverlays;

signals:
    void updateGLSignal();

public slots:
    void mapClicked(QPixmap pixmap , MAP_LAYERS layer);
    void featureClicked(QPixmap pixmap , FEATURE_LAYERS layer);

    void openFileSearchHeightmap(MAP_LAYERS layer);
    void openFileSearchFeaturemap(FEATURE_LAYERS layer);


private slots:

    void updateMap(QPixmap map , MAP_LAYERS layer);
    void updateFeature(QPixmap map , FEATURE_LAYERS layer);

    void on_subdiv_slider_valueChanged(int value);
    void on_simulateBtn_clicked();
    void on_simspeedslider_valueChanged(int value);
    void on_pensizeSlider_valueChanged(int value);

    void on_resetDrawingBtn_clicked();

    void on_opacityValSLider_valueChanged(int value);

    void on_confirmMapBtn_clicked();

    // void on_blackButton_clicked();

    // void on_whiteButton_clicked();

    void on_eraserBtn_clicked();

    void on_greyscaleSlider_valueChanged(int value);

    void on_colorSelectBtn_clicked();

    void on_spawnParticlesBtn_clicked();

    void on_numParticlesSlider_valueChanged(int value);


private:
    void loadDefaultMaps();
    int m_particles = 512;
};
#endif // MAINWINDOW_H
