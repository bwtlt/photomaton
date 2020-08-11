#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QtCore/QTimer>
#include <QtGui/QImage>

static const unsigned int SLIDESHOW_PERIOD = 3000U;

class SlideShow : public QObject
{
    Q_OBJECT
public:
    explicit SlideShow();
    ~SlideShow();

    bool init();
    void resume();
    void pause();

    void addImageToList(const QString& filename);

private:
    bool m_running;
    unsigned char *m_imageBuffer;

    QStringList m_imagesList;
    QStringList::ConstIterator m_imageIt;

    QTimer *m_timer;

signals:
    void handleImage(QImage &image);
    void finished();

public slots:
    void grabImages();
    void stopGrabbing();

};

#endif // SLIDESHOW_H
