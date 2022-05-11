#include <math.h>
#include "pgmdata.h"

static pgmImage* initialiseReduced(pgmImage *image, int factor)
{
    /* 
     * Calculate the dimensions of the reduced image, taking into account remainder
     * pixels after reduction by the factor, using the ceil function to do so.
     */
    int reducedWidth = ceil(getWidth(image) / (double)factor);
    int reducedHeight = ceil(getHeight(image) / (double)factor);

    // Initialise the image.
    pgmImage *reduced = createEmptyImage(reducedWidth, reducedHeight, getMaxGrayValue(image), determineFormat(image));

    return reduced;
} 

pgmImage* reduce(pgmImage *inputImage, int factor)
{
    // Initialise reduced image using the input image and factor.
    pgmImage *reducedImage = initialiseReduced(inputImage, factor);

    unsigned char pixel = 0;
    int row;
    int column;
    int smallerRow = 0;
    int smallerColumn = 0;

    for (row = 0; row < getHeight(inputImage); row++)
    {
        if (smallerRow > getHeight(reducedImage) - 1)
        {
            break;
        }

        for (column = 0; column < getWidth(inputImage); column++)
        {
            pixel = getPixel(inputImage, row, column);
            
            if (row % factor == 0 && column % factor == 0)
            {
                setPixel(reducedImage, pixel, smallerRow, smallerColumn);
                smallerColumn++;

                if (smallerColumn > getWidth(reducedImage) - 1)
                    smallerRow++;
            }
        }

        smallerColumn = 0;
    }

    return reducedImage;
}
