#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/resources/css/common.css");

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
