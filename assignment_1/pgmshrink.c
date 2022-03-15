#include "pgmdata.h"

static pgmImage* initialiseReduced(pgmImage *image, int factor)
{
    pgmImage *reduced = createImage();
    setMagicNumber(reduced, getMagicNumber(image), determineFormat(image));
    setDimensions(reduced, (getWidth(image) / factor), (getHeight(image) / factor));
    setMaxGrayValue(reduced, getMaxGrayValue(image));
    // Comments?
    initImageRaster(reduced);

    return reduced;
} 

pgmImage* reduce(pgmImage *inputImage, int factor)
{
    // Initialise reduced image using the input image and factor.
    pgmImage *reducedImage = initialiseReduced(inputImage, factor);

    unsigned short pixel = 0;
    int count = 0;
    int x;
    int y;
    for (x = 0; x < getHeight(inputImage); x++)
    {
        for (y = 0; y < getWidth(inputImage); y++)
        {
            pixel = getPixel(inputImage, x, y);
            
            if (x % factor == 0 && y % factor == 0 && count <= getWidth(reducedImage) * getHeight(reducedImage))
            {
                setPixel(reducedImage, pixel, count);
                count++;
            }
        }
    }

    return reducedImage;
}