#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

class Image
{
public:
	Image() : m_qImage() {}
	~Image() {}

	const QImage& getQImage() { return m_qImage; }
	void setQImage(QImage& qImg) { m_qImage = qImg; }

	int height() { return m_qImage.height(); }	
	int width() { return m_qImage.width(); }	

private:
	QImage m_qImage;

};

#endif // IMAGE_H