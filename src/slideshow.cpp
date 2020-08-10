#include "slideshow.h"
#include "image.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>

#include <fstream>
#include <iostream>
#include <unistd.h>

SlideShow::SlideShow() :
    m_running(false),
    m_pause(false)
{
    qRegisterMetaType<QImage>("QImage&");
}

SlideShow::~SlideShow()
{
}

bool SlideShow::init()
{
    qDebug() << Q_FUNC_INFO;
    m_running = false;

    return true;
}

void SlideShow::grabImages()
{
    QDir directory(SAVE_FOLDER);
    directory.setNameFilters(QStringList()<<"*.png");
    m_imagesList = directory.entryList();
    QStringList::ConstIterator it = m_imagesList.constBegin();

    while(m_running)
    {
        m_sync.lock();
        if(m_pause)
        {
            m_pauseCond.wait(&m_sync);
        }
        m_sync.unlock();

        if (false ==  m_imagesList.isEmpty())
        {
            QImage image = QImage(SAVE_FOLDER + "/" + *it);

            emit handleImage(image);

            ++it;
            if (m_imagesList.constEnd() == it)
            {
                qDebug() << Q_FUNC_INFO << "reached end of list";
                it = m_imagesList.constBegin();
            }
        }
        qApp->processEvents();
        sleep(3);
    }
}

void SlideShow::stopGrabbing()
{
    qDebug() << Q_FUNC_INFO;
    m_running = false;

    emit finished();
}

void SlideShow::pause()
{
    qDebug() << Q_FUNC_INFO;
    m_sync.lock();
    m_pause = true;
    m_sync.unlock();
}

void SlideShow::resume()
{
    qDebug() << Q_FUNC_INFO;
    m_sync.lock();
    m_pause = false;
    m_running = true;
    m_sync.unlock();
    m_pauseCond.wakeAll();
}

void SlideShow::addImageToList(const QString& filename)
{
    qDebug() << Q_FUNC_INFO << "Filename:" << filename;
    m_imagesList.append(filename);
    qDebug() << Q_FUNC_INFO << "List size:" << QString(m_imagesList.size());
}
