#include "image.h"
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QDebug>


void Image::save()
{
    QDir directory(SAVE_FOLDER);
    if (false == directory.exists())
    {
        qDebug() << "create path" << SAVE_FOLDER;
        directory.mkpath(SAVE_FOLDER);
    }

    QString filename( SAVE_FOLDER + "/" + QDateTime::currentDateTime().toString("ddMMyy_hhmmss") + ".png");
    qDebug() << "save image" << filename;
    assert(m_qImage.save(filename));
}
