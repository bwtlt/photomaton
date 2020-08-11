#include "slideshow.h"
#include "image.h"
#include "utils.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTime>
#include <QtWidgets/QApplication>

#include <fstream>
#include <iostream>
#include <unistd.h>

SlideShow::SlideShow() :
    m_running(false)
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
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(grabImages()));

    QDir directory(SAVE_FOLDER);
    directory.setNameFilters(QStringList()<<"*.png");
    m_imagesList = directory.entryList();

    qDebug() << Q_FUNC_INFO << QString::number(m_imagesList.size()) << "images in slideshow";

    m_imageIt = m_imagesList.constBegin();

    m_timer->start(SLIDESHOW_PERIOD);
    return true;
}

void SlideShow::grabImages()
{
    if (false ==  m_imagesList.isEmpty() && true == m_running)
    {
        qDebug() << Q_FUNC_INFO << "display image" << QString(*m_imageIt);
        QImage image = QImage(SAVE_FOLDER + "/" + *m_imageIt);

        emit handleImage(image);

        ++m_imageIt;
        if (m_imagesList.constEnd() == m_imageIt)
        {
            qDebug() << Q_FUNC_INFO << "reached end of list";
            m_imageIt = m_imagesList.constBegin();
        }
        qApp->processEvents();
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
    m_running = false;
}

void SlideShow::resume()
{
    qDebug() << Q_FUNC_INFO;
    m_running = true;
}

void SlideShow::addImageToList(const QString& filename)
{
    qDebug() << Q_FUNC_INFO << "Filename:" << filename;
    m_imagesList.append(filename);
    qDebug() << Q_FUNC_INFO << "List size:" << QString::number(m_imagesList.size());
}

