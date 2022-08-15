#include <QApplication>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*

#include <QApplication>
#include <QPushButton>
#include <Qt3DCore>
#include <Qt3DExtras>

 */