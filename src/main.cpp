#include "mainwindow.h"

#include <QtWidgets/QApplication>
#include <QtCore/QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.startPhotomaton();
    w.showFullScreen();

    return a.exec();
}
