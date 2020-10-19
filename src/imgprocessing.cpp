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

    cv::Mat output;
    cv::Mat kernel =
        (cv::Mat_<float>(3, 3)
            <<
            0.272, 0.534, 0.131,
            0.349, 0.686, 0.168,
            0.393, 0.769, 0.189);
    cv::transform(input, output, kernel);

    QImage dest(static_cast<const uchar*>(output.data), output.cols, output.rows, output.step, QImage::Format_RGB888);
    dest.bits();

    destination.setQImage(dest);
}
