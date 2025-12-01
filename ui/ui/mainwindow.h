#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend.h"
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
    void mapClicked();
    void setHeightMap(QString filename);
    void returnClicked();
    void openFileSearch();

private slots:
    void on_subdiv_slider_valueChanged(int value);

    void on_simulateBtn_clicked();

    void on_simspeedslider_valueChanged(int value);

private:
};
#endif // MAINWINDOW_H
