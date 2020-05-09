#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>

#include "videostream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startPreview();

private:
    Ui::MainWindow *ui;
    bool m_cameraRunning;

    QThread *m_workerThread;
    VideoStream *m_worker;

private slots:
    void handleImage(QImage &image);
    void cameraFinished();
};

#endif // MAINWINDOW_H
