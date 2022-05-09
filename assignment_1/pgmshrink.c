#include <math.h>
#include "pgmdata.h"

static pgmImage* initialiseReduced(pgmImage *image, int factor)
{
    pgmImage *reduced = createImage();
    setMagicNumber(reduced, getMagicNumber(image), determineFormat(image));

    /* 
     * Calculate the dimensions of the reduced image, taking into account remainder
     * pixels after reduction by the factor, using the ceil function to do so.
     */
    int reducedWidth = ceil(getWidth(image) / (double)factor);
    int reducedHeight = ceil(getHeight(image) / (double)factor);

    // Set these dimensions.
    setDimensions(reduced, reducedWidth, reducedHeight);
    setMaxGrayValue(reduced, getMaxGrayValue(image));

    // We now have enough information to initialise the raster.
    initImageRaster(reduced);

    return reduced;
} 

pgmImage* reduce(pgmImage *inputImage, int factor)
{
    // Initialise reduced image using the input image and factor.
    pgmImage *reducedImage = initialiseReduced(inputImage, factor);

    unsigned char pixel = 0;
    int count = 0;
    int row;
    int column;
    for (row = 0; row < getHeight(inputImage); row++)
    {
        for (column = 0; column < getWidth(inputImage); column++)
        {
            pixel = getPixel(inputImage, row, column);
            
            if (row % factor == 0 && column % factor == 0)
            {
                setPixel(reducedImage, pixel, count);
                count++;
            }
        }
    }

    return reducedImage;
}
