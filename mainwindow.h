#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>

#include "videostream.h"
#include "gpio.h"

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

    QThread *m_cameraWorkerThread;

    VideoStream *m_cameraWorker;

    QImage m_image;

    typedef enum {
        STATE_OFF,
        STATE_PREVIEW,
        STATE_CAPTURESETTINGS,
        STATE_FINAL
    } State;

    State m_currentState;

private:
    void captureImage();
    void resumePreview();

private slots:
    void handleImage(QImage &image);
    void cameraFinished();

    void okBtnPressed();
    void cancelBtnPressed();
    void leftBtnPressed();
    void rightBtnPressed();
};

#endif // MAINWINDOW_H
