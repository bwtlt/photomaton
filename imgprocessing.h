#ifndef IMGPROCESSING_H
#define IMGPROCESSING_H

#include <opencv2/core/core.hpp>
#include <QImage>

class ImgProcessing
{
public:
    ImgProcessing();
    ~ImgProcessing();

    static QImage toGray(QImage& image);
    static QImage toSepia(QImage& image);
};

#endif // IMGPROCESSING_H
