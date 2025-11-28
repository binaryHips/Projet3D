#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "backend.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MapCPU heightmap = Backend::loadHeightmap(":/test.png");
    context.addMap(std::move(heightmap));

    ui->setupUi(this);
    // addStretch() to make them hug top
    ui->gl_settings_layout->addStretch();
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


MainWindow::~MainWindow()
{
    delete ui;
}
