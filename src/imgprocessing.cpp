#include "imgprocessing.h"

#include <opencv2/imgproc.hpp>
#include <QtCore/QDebug>

void toGray(Image& source, Image& destination)
{
    qDebug() << Q_FUNC_INFO;
    QImage qImg = source.getQImage();
    cv::Mat input(qImg.height(), qImg.width(), CV_8UC3, static_cast<uchar*>(qImg.bits()), qImg.bytesPerLine());

    cv::Mat output;
    cv::cvtColor(input, output, CV_BGR2GRAY);

    QImage dest(static_cast<const uchar*>(output.data), output.cols, output.rows, output.step, QImage::Format_Grayscale8);
    dest.bits();

    destination.setQImage(dest);
}

void toSepia(Image& source, Image& destination)
{
    qDebug() << Q_FUNC_INFO;
    QImage qImg = source.getQImage();
    cv::Mat input(qImg.height(), qImg.width(), CV_8UC3, static_cast<uchar*>(qImg.bits()), qImg.bytesPerLine());

    QImage dest(static_cast<const uchar*>(input.data), input.cols, input.rows, input.step, QImage::Format_RGB888);
    dest.bits();

    destination.setQImage(dest);
}
