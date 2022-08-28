#include "include/mainwindow.h"
#include "../ui_mainwindow.h"
#include "../include/CustomOpenGLWidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->canvas = new CustomOpenGLWidget(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
