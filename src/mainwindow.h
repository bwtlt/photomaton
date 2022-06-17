#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videostream.h"
#include "slideshow.h"
#include "gpio.h"
#include "image.h"

#include <QtWidgets/QMainWindow>

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

    VideoStream *m_cameraWorker;
    SlideShow *m_slideShow;

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

    QTimer* m_idleTimer;

private:
    void captureImage();
    void resumePreview();
    void startSlideShow();
    void saveImage();

private slots:
    void handleCameraImage(QImage &image);
    void handleSSImage(QImage &image);
    void cameraFinished();

    void okBtnPressed();
    void cancelBtnPressed();
    void leftBtnPressed();
    void rightBtnPressed();

    void idleTimedOut();
};

#endif // MAINWINDOW_H
