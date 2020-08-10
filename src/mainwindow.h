#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videostream.h"
#include "slideshow.h"
#include "gpio.h"
#include "image.h"
#include "imgprocessing.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startPhotomaton();

private:
    Ui::MainWindow *ui;
    QFont font;
    bool m_cameraRunning;
    bool m_slideShowRunning;

    QThread *m_cameraWorkerThread;
    QThread *m_slideShowWorkerThread;

    VideoStream *m_cameraWorker;
    SlideShow *m_slideShowWorker;

    Image m_image;
    Image m_rawImage;

    typedef enum {
        STATE_OFF,
        STATE_SLIDESHOW,
        STATE_PREVIEW,
        STATE_CAPTURESETTINGS,
        STATE_FINAL
    } State;

    State m_currentState;

private:
    void captureImage();
    void resumePreview();
    void startSlideShow();
    void applyFilter(eFilter filter);
    void saveImage();

private slots:
    void handleCameraImage(QImage &image);
    void handleSSImage(QImage &image);
    void cameraFinished();

    void okBtnPressed();
    void cancelBtnPressed();
    void leftBtnPressed();
    void rightBtnPressed();
};

#endif // MAINWINDOW_H
