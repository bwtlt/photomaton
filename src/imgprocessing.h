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

void toGray(Image& image);
void toSepia(Image& image);

#endif // IMGPROCESSING_H
