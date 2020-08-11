#ifndef IMAGE_H
#define IMAGE_H

#include <QtGui/QImage>

static const QString SAVE_FOLDER("/home/pi/photomaton2/saved_images");

class Image
{
public:
	Image() : m_qImage(), m_savePath() {}
	~Image() {}

	const QImage& getQImage() { return m_qImage; }
	void setQImage(QImage& qImg) { m_qImage = qImg; }

	int height() { return m_qImage.height(); }	
	int width() { return m_qImage.width(); }	

    const QString& getSavePath() { return m_savePath; }

    void save();

private:
	QImage m_qImage;

    QString m_savePath;

};

#endif // IMAGE_H
