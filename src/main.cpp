#include "mainwindow.h"

#include <QtWidgets/QApplication>
#include <QtCore/QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.startPreview();
    w.showFullScreen();

    return a.exec();
}
