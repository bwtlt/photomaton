#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <raspicam/raspicam.h>
#include <QtGui/QImage>
#include <QtCore/QTimer>

static const unsigned int FAST_FRAMERATE = 50U;
static const unsigned int SLOW_FRAMERATE = 300U;

class VideoStream : public QObject
{
    Q_OBJECT
public:
    explicit VideoStream();
    ~VideoStream();

    bool openCamera();
    void resume();
    void pause();

    void setRefreshInterval(unsigned int interval);

    void setEffect(raspicam::RASPICAM_IMAGE_EFFECT effect);

private:
    raspicam::RaspiCam m_camera;
    bool m_running;
    unsigned char *m_imageBuffer;

    QTimer *m_timer;

signals:
    void handleImage(QImage &image);
    void finished();

public slots:
    void grabImages();
    void stopGrabbing();
};

#endif // VIDEOSTREAM_H
