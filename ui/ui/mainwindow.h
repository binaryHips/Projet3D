#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend.h"
#include "mapdrawwindow.h"
#include "scribblearea.h"
#include "glwidget.h"
#include "includes_all_cpu.h"


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

public slots:
    void mapClicked(QPixmap pixmap);
    void setHeightMap(QString filename);
    void openFileSearch();

private slots:

    void updateMap(QPixmap map , MAP_LAYERS layer);

    void on_subdiv_slider_valueChanged(int value);

    void on_simulateBtn_clicked();

    void on_simspeedslider_valueChanged(int value);

    void on_pensizeSlider_valueChanged(int value);

    void on_resetDrawingBtn_clicked();

    void on_opacityValSLider_valueChanged(int value);

    void on_confirmMapBtn_clicked();

private:
    // MapDrawWindow *drawPage; TODO add eventually
};
#endif // MAINWINDOW_H
