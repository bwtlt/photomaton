#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QThread>

#include "videostream.h"
#include "gpio.h"
#include "image.h"
#include "imgprocessing.h"

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

private slots:
    void handleImage(QImage &image);
    void cameraFinished();

    void okBtnPressed();
    void cancelBtnPressed();
    void leftBtnPressed();
    void rightBtnPressed();
};

#endif // MAINWINDOW_H
