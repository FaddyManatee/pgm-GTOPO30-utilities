#include "pgmdata.h"

static pgmImage* initialiseReduced(pgmImage *image, int factor)
{
    pgmImage *reduced = createImage();
    setMagicNumber(reduced, getMagicNumber(image), determineFormat(image));
    setDimensions(reduced, (getWidth(image) / factor), (getHeight(image) / factor));
    setMaxGrayValue(reduced, getMaxGrayValue(image));

    // We now have enough information to initialise the raster.
    initImageRaster(reduced);

    return reduced;
} 

pgmImage* reduce(pgmImage *inputImage, int factor)
{
    // Initialise reduced image using the input image and factor.
    pgmImage *reducedImage = initialiseReduced(inputImage, factor);

    unsigned short pixel = 0;
    int count = 0;
    int row;
    int column;
    for (row = 0; row < getHeight(inputImage); row++)
    {
        for (column = 0; column < getWidth(inputImage); column++)
        {
            pixel = getPixel(inputImage, row, column);
            
            if (row % factor == 0 && column % factor == 0 && count <= getWidth(reducedImage) * getHeight(reducedImage))
            {
                setPixel(reducedImage, pixel, count);
                count++;
            }
        }
    }

    return reducedImage;
}
