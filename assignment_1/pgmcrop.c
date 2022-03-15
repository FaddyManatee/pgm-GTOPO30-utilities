#include "pgmdata.h"

pgmImage* reduce(pgmImage *inputImage, int factor)
{
    // Get the raster of the input image.
    unsigned short* raster = getRaster(inputImage);
}