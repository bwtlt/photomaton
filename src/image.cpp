#include "image.h"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QDebug>


void Image::save()
{
    QDir directory(SAVE_FOLDER);
    if (false == directory.exists())
    {
        qDebug() << Q_FUNC_INFO << "Output folder does not exist, creating path" << SAVE_FOLDER;
        directory.mkpath(SAVE_FOLDER);
    }

    QString filename( SAVE_FOLDER + "/" + QDateTime::currentDateTime().toString("ddMMyy_hhmmss") + ".png");
    qDebug() << Q_FUNC_INFO << "save image" << filename;
    assert(m_qImage.save(filename));
}
