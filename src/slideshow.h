#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QImage>
#include <QMutex>
#include <QWaitCondition>

class SlideShow : public QObject
{
    Q_OBJECT
public:
    explicit SlideShow();
    ~SlideShow();

    bool init();
    void resume();
    void pause();

private:
    bool m_running;
    bool m_pause;
    unsigned char *m_imageBuffer;
    QMutex m_sync;
    QWaitCondition m_pauseCond;

    QStringList m_imagesList;

signals:
    void handleImage(QImage &image);
    void finished();

public slots:
    void grabImages();
    void stopGrabbing();

    void addImageToList(const QString& filename);
};

#endif // SLIDESHOW_H
