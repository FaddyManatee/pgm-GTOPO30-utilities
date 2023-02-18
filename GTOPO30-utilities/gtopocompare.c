#include "gtopodata.h"


/*
 * Checks if the same elevation point from each DEM file both have the value.
 * Returns 1 if both are identical, 0 otherwise.
 */
static int compareRasters(signed short **rasterOne, signed short **rasterTwo, int width, int height)
{
    int row;
    int column;

    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column++)
        {
            if (rasterOne[row][column] != rasterTwo[row][column])
                return 0;
        }
    }

    // Return 1 to indicate no pixels were different, hence the images are the same.
    return 1;
}


/*
 * Compares two DEM files and determines if they are logically equivalent
 * (both describe the same image regardless of formatting). Returns 1 if both
 * files are logically equivalent, returning 0 if they are different. Two files
 * are logically equivalent if all corresponding elevation points in both files
 * have the same value. Width and height equivalence is guarenteed by CLI.
 */
int compare(gtopoDEM *firstFile, gtopoDEM *secondFile)
{
    // Get the image raster data from both input images.
    signed short** rasterDEMOne = getRaster(firstFile);
    signed short** rasterDEMTwo = getRaster(secondFile);

    // Compare the rasters pixel by pixel and return the result of the comparison.
    return compareRasters(rasterDEMOne, rasterDEMTwo, getWidth(firstFile), getHeight(firstFile));
}
