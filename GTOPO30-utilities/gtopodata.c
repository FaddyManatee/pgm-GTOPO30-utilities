#include <stdlib.h>
#include "gtopolimits.h"

// Enable this #define directive for testing memory allocation failure.
// #define malloc(...) NULL


// http://netpbm.sourceforge.net/doc/pgm.html 


/*
 * Stores all required data related to a DEM (Digital Elevation Model) that is to
 * be handled by a program. Properties of a DEM include:
 * 
 * Width and height: The width and height of the raster of the DEM which consists
 * of elevation points, each being a 16-bit signed integer stored in big-endian
 * format.
 * 
 * Raster: Each elevation point in the raster represents an elevation between
 * -407 and 8752 inclusive. Each is a signed integer of 16-bits - a signed short.
 * Each elevation occupies 2 bytes in big-endian format.
 */
typedef struct DEM
{
    int width; 
    int height;
    signed short **raster;
} DEM;


/*
 * Dynamically allocates memory to an DEM structure and returns a pointer to
 * the allocated memory. If memory allocation fails, the DEM and its raster are
 * set to NULL.
 */
DEM* createDEM(int width, int height)
{
    // Allocate memory to a new image and set initial NULL/empty values. 
    DEM *newDEM = (DEM *) malloc(sizeof(DEM));

    // Check for failed memory allocation and return NULL if this occurred.
    if (newDEM == NULL)
        return NULL;

    newDEM->width = width;
    newDEM->height = height;
    newDEM->raster = (signed short **) malloc(sizeof(signed short *) * height);

    if (newDEM->raster == NULL)
        return;

    // Allocate memory to each pixel in the raster and set them to have no data.
    int row;
    int column;

    for (row = 0; row < height; row++)
    {
        newDEM->raster[row] = (signed short *) malloc(sizeof(signed short) * width);

        if (newDEM->raster[row] == NULL)
        {
            newDEM->raster = NULL;
            return;
        }
    }

    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column++)
        {
            newDEM->raster[row][column] = NO_DATA;
        }
    }

    return newDEM;
}


int getWidth(DEM *targetDEM)
{
    return targetDEM->width;
}


int getHeight(DEM *targetDem)
{
    return targetDem->height;
}


signed short** getRaster(DEM *targetDEM)
{
    return targetDEM->raster;
}


/*
 * Returns the value of the elevation in the raster given a pointer to the DEM and
 * the row and column this elevation should come from.
 */
unsigned char getElevation(DEM *targetDEM, int row, int column)
{
    return targetDEM->raster[row][column];
}


/*
 *
 */
void setElevation(DEM *targetDEM, signed short value, int row, int column)
{
    targetDEM->raster[row][column] = value;
}


/*
 * Frees all dynamically allocated data related to an image given its pointer
 * which includes the raster and comments.
 */
void freeDEM(DEM *targetDEM)
{
    // Only free if the DEM is initialised.
    if (targetDEM != NULL)
    {
        // Free memory allocated to the image raster if it was allocated.
        if (targetDEM->raster != NULL)
        {
            int x;
            for (x = 0; x < targetDEM->height; x++)
            {
                free(targetDEM->raster[x]);
            }
            free(targetDEM->raster);
        }

        free(targetDEM);
    }
}
