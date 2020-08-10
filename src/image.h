#ifndef IMAGE_H
#define IMAGE_H

#include <QtGui/QImage>

static const QString SAVE_FOLDER("/home/pi/photomaton2/saved_images");

class Image
{
public:
	Image() : m_qImage() {}
	~Image() {}

	const QImage& getQImage() { return m_qImage; }
	void setQImage(QImage& qImg) { m_qImage = qImg; }

	int height() { return m_qImage.height(); }	
	int width() { return m_qImage.width(); }	

    void save();

private:
	QImage m_qImage;

};

#endif // IMAGE_H
