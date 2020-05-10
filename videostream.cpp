#include "videostream.h"

#include <fstream>
#include <iostream>

#include <QDebug>
#include <QApplication>

#include <unistd.h>

VideoStream::VideoStream() :
    m_running(false),
    m_pause(false)
{
    qRegisterMetaType<QImage>("QImage&");
    m_imageBuffer = new unsigned char[m_camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
}

VideoStream::~VideoStream()
{
}

bool VideoStream::openCamera()
{
    bool status = m_camera.open();
    if (!status)
    {
        qDebug() << "Error opening camera";
    }
    else
    {
        sleep(3); // wait for camera to stabilize
        m_running = true;
    }

    return status;
}

void VideoStream::grabImages()
{
    while(m_running)
    {
        m_sync.lock();
        if(m_pause)
        {
            qDebug() << "Thread paused";
            m_pauseCond.wait(&m_sync);
            qDebug() << "Thread resumed";
        }
        m_sync.unlock();

        m_camera.grab();
        m_camera.retrieve(m_imageBuffer, raspicam::RASPICAM_FORMAT_IGNORE);

        QImage image = QImage(m_imageBuffer,
                              static_cast<int>(m_camera.getWidth()),
                              static_cast<int>(m_camera.getHeight()),
                              QImage::Format_RGB888).rgbSwapped();

        emit handleImage(image);

        qApp->processEvents();
        usleep(100);
    }
}

void VideoStream::stopGrabbing()
{
    m_running = false;

    emit finished();
}

void VideoStream::pause()
{
    m_sync.lock();
    m_pause = true;
    m_sync.unlock();
}

void VideoStream::resume()
{
    m_sync.lock();
    m_pause = false;
    m_sync.unlock();
    m_pauseCond.wakeAll();
}
