#ifndef IMGPROCESSING_H
#define IMGPROCESSING_H

#include <opencv2/core/core.hpp>
#include "image.h"

typedef enum
{
    NONE,
    BLACK_AND_WHITE,
    SEPIA
} eFilter ;

void toGray(Image& src, Image& dest);
void toSepia(Image& src, Image& dest);

#endif // IMGPROCESSING_H
