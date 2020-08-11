#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <raspicam/raspicam.h>
#include <QtGui/QImage>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

class VideoStream : public QObject
{
    Q_OBJECT
public:
    explicit VideoStream();
    ~VideoStream();

    bool openCamera();
    void resume();
    void pause();

    void setEffect(raspicam::RASPICAM_IMAGE_EFFECT effect);

private:
    raspicam::RaspiCam m_camera;
    bool m_running;
    bool m_pause;
    unsigned char *m_imageBuffer;
    QMutex m_sync;
    QWaitCondition m_pauseCond;

signals:
    void handleImage(QImage &image);
    void finished();

public slots:
    void grabImages();
    void stopGrabbing();
};

#endif // VIDEOSTREAM_H
