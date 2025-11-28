#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    GeoContextCPU context = GeoContextCPU::createGeoContext();

    ~MainWindow();

    Ui::MainWindow *ui;

public slots:
    void mapClicked();
    void returnClicked();

private:
};
#endif // MAINWINDOW_H
