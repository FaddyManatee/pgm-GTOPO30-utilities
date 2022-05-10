#include "pgmdata.h"


/*
 * Checks if the same pixel from each image both have the same gray value. Returns 1
 * if 1 if identical, 0 otherwise.
 */
static int compareRasters(unsigned char *rasterOne, unsigned char *rasterTwo, int pixels)
{
    int x;
    for (x = 0; x < pixels; x++)
    {
        if (rasterOne[x] != rasterTwo[x])
            return 0;
    }

    return 1;
}


/*
 * Compares two pgm images and determines if they are logically equivalent
 * (both describe the same image regardless of formatting). Returns 1 if both
 * images are logically equivalent, returning 0 if they are different. Two images
 * are logically equivalent if their dimensions match, maximum gray values match
 * (as the brightness of each pixel is proportional to this number), and if the same
 * pixel from each image both have the same gray value.
 */
int compare(pgmImage *imageOne, pgmImage *imageTwo)
{
    // Get the image width from both input images.
    int widthImageOne = getWidth(imageOne);
    int widthImageTwo = getWidth(imageTwo);

    // Return 0 if the width values are different.
    if (widthImageOne != widthImageTwo)
        return 0;

    // Get the image height from both input images.
    int heightImageOne = getHeight(imageOne);
    int heightImageTwo = getHeight(imageTwo);

    // Return 0 if the height values are different.
    if (heightImageOne != heightImageTwo)
        return 0;

    /*
     * We now know the dimensions of both images are the same, so we can calculate
     * the number of pixels that appear in both image. We use the dimensions of
     * image 1 here.
     */
    int numberOfPixels = widthImageOne * heightImageOne;

    // Get the maximum gray value from both input images.
    int maxGrayImageOne = getMaxGrayValue(imageOne);
    int maxGrayImageTwo = getMaxGrayValue(imageTwo);

    // Return 0 if the maximum gray values are different.
    if (maxGrayImageOne != maxGrayImageTwo)
        return 0;

    // Get the image raster data from both input images.
    unsigned char* rasterImageOne = getRaster(imageOne);
    unsigned char* rasterImageTwo = getRaster(imageTwo);

    // Compare the rasters pixel by pixel and return the result of the comparison.
    return compareRasters(rasterImageOne, rasterImageTwo, numberOfPixels);
}
