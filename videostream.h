#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <raspicam/raspicam.h>
#include <QImage>

class VideoStream : public QObject
{
    Q_OBJECT
public:
    explicit VideoStream();
    ~VideoStream();

    bool openCamera();

private:
    raspicam::RaspiCam m_camera;
    bool m_running;
    unsigned char *m_imageBuffer;

signals:
    void handleImage(QImage &image);
    void finished();

public slots:
    void grabImages();
    void stopGrabbing();
};

#endif // VIDEOSTREAM_H
