#include <stdio.h>
#include <stdlib.h>
#include "pgmdata.h"
#include "pgmerror.h"

typedef struct point
{
    int widthCoord;
    int heightCoord;
} point;


static pgmImage*** createTiles(pgmImage *image, int factor)
{
    // Calculate the width of right-most tiles.
    int tileRightWidth = (getWidth(image) / factor) + (getWidth(image) % factor);
    
    // Calculate the height of bottom-most tiles.
    int tileBottomHeight = (getHeight(image) / factor) + (getHeight(image) % factor);

    // Calculate width and height of pixels that aren't at on the very right or bottom.
    int tileWidth = (getWidth(image) / factor);
    int tileHeight = (getHeight(image) / factor);

    // Create nxn image structs.
    int row;
    int column;

    pgmImage ***tiles = (pgmImage ***) malloc(sizeof(pgmImage **) * factor);

    for (row = 0; row < factor; row++)
    {
        tiles[row] = (pgmImage **) malloc(sizeof(pgmImage *) * factor);
    }
    
    // Initialise the tiles and set their dimensions.
    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            if (row < factor - 1 && column < factor - 1)
            {
                tiles[row][column] = createEmptyImage(tileWidth, tileHeight,
                    getMaxGrayValue(image), determineFormat(image));
            }
            else if (row < factor - 1 && column == factor - 1)
            {
                tiles[row][column] = createEmptyImage(tileRightWidth, tileHeight, 
                    getMaxGrayValue(image), determineFormat(image));
            }
            else if (row == factor - 1 && column < factor - 1)
            {
                tiles[row][column] = createEmptyImage(tileWidth, tileBottomHeight,
                    getMaxGrayValue(image), determineFormat(image));   
            }
            else if (row == factor - 1 && column == factor - 1)
            {
                tiles[row][column] = createEmptyImage(tileRightWidth, tileBottomHeight,
                    getMaxGrayValue(image), determineFormat(image));
            }
        }
    }
    
    return tiles;
}


static point** calculateReadPoints(pgmImage *image, pgmImage ***tiles, int factor)
{
    // Allocate memory to the coordinates where each tile begins.
    point **points = (point **) malloc(sizeof(point *) * factor);

    int row;
    int column;

    for (row = 0; row < factor; row++)
    {
        points[row] = (point *) malloc(sizeof(point) * factor);
    }

    int currentWidth = 0;
    int currentHeight = 0;

    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {   
            points[row][column].widthCoord = currentWidth;
            points[row][column].heightCoord = currentHeight;

            currentWidth = currentWidth + getWidth(tiles[row][column]);
        }

        currentWidth = 0;
        currentHeight = currentHeight + getHeight(tiles[row][0]);
    }

    return points;
}


pgmImage*** tile(pgmImage *image, int factor)
{
    /* 
     * We need to split the input image into (factor * factor) smaller images. We
     * need an array of (factor * factor) image structs.
     */
    pgmImage ***imageTiles = createTiles(image, factor);

    // Calculate the coordinates where each tile begins (top-left corner).
    point **readPoints = calculateReadPoints(image, imageTiles, factor);

    int row;
    int column;
    int subRow;
    int subColumn;

    // Loop over tiles by row and column.
    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            // Loop over pixels within tiles by row and column.
            for (subRow = 0; subRow < getHeight(imageTiles[row][column]); subRow++)
            {
                for (subColumn = 0; subColumn < getWidth(imageTiles[row][column]); subColumn++)
                {
                    // Assign current pixel to corresponding tile.
                    setPixel(imageTiles[row][column], 
                        getPixel(image, readPoints[row][column].heightCoord + subRow,
                            readPoints[row][column].widthCoord + subColumn),
                        subRow, subColumn);
                }
            }
        }
    }

    // Free memory allocated to read points.
    for (row = 0; row < factor; row++)
    {
        free(readPoints[row]);
    }
    free(readPoints);

    return imageTiles;
}


/*
 * Adds the child image to the parent image with the top-left corner of the image
 * placed at the specified row and column values. Returns 0 on success and 1 on
 * failure if pixels of a sub-image are placed outside of the larger image.
 */
int addImage(pgmImage *parent, pgmImage *child, int startRow, int startColumn)
{
    int parentRow;
    int parentColumn;
    int childRow = 0;
    int childColumn = 0;
    int pixelsWritten = 0;

    for (parentRow = startRow; parentRow < getHeight(parent); parentRow++)
    {
        if (startRow + childRow > getHeight(parent) || childRow == getHeight(child))
        {
            break;
        }

        for (parentColumn = startColumn; parentColumn < getWidth(parent); parentColumn++)
        {
            if (startColumn + childColumn > getWidth(parent) || childColumn == getWidth(child))
            {
                break;
            }

            setPixel(parent, getPixel(child, childRow, childColumn), parentRow, parentColumn);
            childColumn++;
            pixelsWritten++;
        }

        childRow++;
        childColumn = 0;
    }

    // Check that all pixels of the child/sub-image were written.
    if (pixelsWritten != getWidth(child) * getHeight(child))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
