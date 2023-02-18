#include <math.h>
#include "gtopodata.h"

static gtopoDEM* initialiseReduced(gtopoDEM *inputDEM, int factor)
{
    /* 
     * Calculate the dimensions of the reduced image, taking into account remainder
     * pixels after reduction by the factor, using the ceil function to do so.
     */
    int reducedWidth = ceil(getWidth(inputDEM) / (double)factor);
    int reducedHeight = ceil(getHeight(inputDEM) / (double)factor);

    // Initialise the image.
    gtopoDEM *reduced = createDEM(reducedWidth, reducedHeight);

    return reduced;
} 

gtopoDEM* reduce(gtopoDEM *inputDEM, int factor)
{
    // Initialise reduced image using the input image and factor.
    gtopoDEM *reducedDEM = initialiseReduced(inputDEM, factor);

    signed short elevation = 0;
    int row;
    int column;
    int smallerRow = 0;
    int smallerColumn = 0;

    for (row = 0; row < getHeight(inputDEM); row++)
    {
        if (smallerRow > getHeight(reducedDEM) - 1)
        {
            break;
        }

        for (column = 0; column < getWidth(inputDEM); column++)
        {
            elevation = getElevation(inputDEM, row, column);
            
            if (row % factor == 0 && column % factor == 0)
            {
                setElevation(reducedDEM, elevation, smallerRow, smallerColumn);
                smallerColumn++;

                if (smallerColumn > getWidth(reducedDEM) - 1)
                    smallerRow++;
            }
        }

        smallerColumn = 0;
    }

    return reducedDEM;
}
