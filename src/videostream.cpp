#include "videostream.h"

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

#include <fstream>
#include <iostream>
#include <unistd.h>

static unsigned int CAPTURE_WIDTH = 1280;
static unsigned int CAPTURE_HEIGHT = 720;

VideoStream::VideoStream() :
    m_running(false)
{
    qRegisterMetaType<QImage>("QImage&");
    m_imageBuffer = new unsigned char[m_camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
}

VideoStream::~VideoStream()
{
}

bool VideoStream::openCamera()
{
    qDebug() << Q_FUNC_INFO << "Opening camera...";
    bool status = m_camera.open();
    if (!status)
    {
        qDebug() << Q_FUNC_INFO << "Error opening camera";
    }
    else
    {
        sleep(3); // wait for camera to stabilize
        m_running = true;
        m_camera.setCaptureSize(CAPTURE_WIDTH, CAPTURE_HEIGHT);

        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(grabImages()));
        m_timer->start(FAST_FRAMERATE);

        qDebug() << Q_FUNC_INFO << "Camera opened";
    }

    return status;
}

void VideoStream::grabImages()
{
    if (true == m_running)
    {
        m_camera.grab();
        m_camera.retrieve(m_imageBuffer, raspicam::RASPICAM_FORMAT_IGNORE);

        QImage image = QImage(m_imageBuffer,
                              static_cast<int>(m_camera.getWidth()),
                              static_cast<int>(m_camera.getHeight()),
                              QImage::Format_RGB888);//.mirrored();//.rgbSwapped();

        emit handleImage(image);

        qApp->processEvents();
    }
}

void VideoStream::stopGrabbing()
{
    qDebug() << Q_FUNC_INFO;
    m_running = false;
    emit finished();
}

void VideoStream::pause()
{
    qDebug() << Q_FUNC_INFO;
    m_running = false;
}

void VideoStream::resume()
{
    qDebug() << Q_FUNC_INFO;
    m_running = true;
}

void VideoStream::setEffect(raspicam::RASPICAM_IMAGE_EFFECT effect)
{
    qDebug() << Q_FUNC_INFO << "Effect:" << QString(effect);
    m_camera.setImageEffect(effect);
}

void VideoStream::setRefreshInterval(unsigned int interval)
{
    m_timer->setInterval(interval);
}
