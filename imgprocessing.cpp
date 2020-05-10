#include "imgprocessing.h"

#include <opencv2/imgproc.hpp>

ImgProcessing::ImgProcessing()
{

}

ImgProcessing::~ImgProcessing()
{

}

QImage ImgProcessing::toGray(QImage& image)
{
    cv::Mat input(image.height(), image.width(), CV_8UC3, static_cast<uchar*>(image.bits()), image.bytesPerLine());

    cv::Mat output;
    cv::cvtColor(input, output, CV_BGR2GRAY);

    QImage dest(static_cast<const uchar*>(output.data), output.cols, output.rows, output.step, QImage::Format_Grayscale8);
    dest.bits();
    return dest;
}

QImage ImgProcessing::toSepia(QImage& image)
{
    cv::Mat input(image.height(), image.width(), CV_8UC3, static_cast<uchar*>(image.bits()), image.bytesPerLine());

    QImage dest(static_cast<const uchar*>(input.data), input.cols, input.rows, input.step, QImage::Format_RGB888);
    dest.bits();
    return dest;
}
