#ifndef IMGPROCESSING_H
#define IMGPROCESSING_H

#include "image.h"

#include <opencv2/core/core.hpp>

typedef enum
{
    NONE,
    BLACK_AND_WHITE,
    SEPIA
} eFilter ;

void toGray(Image& src, Image& dest);
void toSepia(Image& src, Image& dest);

#endif // IMGPROCESSING_H
