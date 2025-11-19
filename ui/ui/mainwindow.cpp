#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // addStretch() to make them hug top
    ui->gl_settings_layout->addStretch();
}

// TODO : Implement custom feature to resize the images correctly :)
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    std::cout << "Resize" << std::endl;
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
