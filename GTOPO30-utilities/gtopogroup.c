#include <stdio.h>
#include <stdlib.h>
#include "gtopodata.h"
#include "gtopoerror.h"

typedef struct point
{
    int widthCoord;
    int heightCoord;
} point;


static gtopoDEM*** createTiles(gtopoDEM *targetDEM, int factor)
{
    // Calculate the width of right-most tiles.
    int tileRightWidth = (getWidth(targetDEM) / factor) + (getWidth(targetDEM) % factor);
    
    // Calculate the height of bottom-most tiles.
    int tileBottomHeight = (getHeight(targetDEM) / factor) + (getHeight(targetDEM) % factor);

    // Calculate width and height of elevation points that aren't at on the very right or bottom.
    int tileWidth = (getWidth(targetDEM) / factor);
    int tileHeight = (getHeight(targetDEM) / factor);

    // Create nxn DEM file structs.
    int row;
    int column;

    gtopoDEM ***tiles = (gtopoDEM ***) malloc(sizeof(gtopoDEM **) * factor);

    for (row = 0; row < factor; row++)
    {
        tiles[row] = (gtopoDEM **) malloc(sizeof(gtopoDEM *) * factor);
    }
    
    // Initialise the tiles and set their dimensions.
    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            if (row < factor - 1 && column < factor - 1)
            {
                tiles[row][column] = createDEM(tileWidth, tileHeight);
            }
            else if (row < factor - 1 && column == factor - 1)
            {
                tiles[row][column] = createDEM(tileRightWidth, tileHeight);
            }
            else if (row == factor - 1 && column < factor - 1)
            {
                tiles[row][column] = createDEM(tileWidth, tileBottomHeight);   
            }
            else if (row == factor - 1 && column == factor - 1)
            {
                tiles[row][column] = createDEM(tileRightWidth, tileBottomHeight);
            }
        }
    }
    
    return tiles;
}


static point** calculateReadPoints(gtopoDEM *targetDEM, gtopoDEM ***tiles, int factor)
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


gtopoDEM*** tile(gtopoDEM *inputDEM, int factor)
{
    /* 
     * We need to split the input image into (factor * factor) smaller images. We
     * need an array of (factor * factor) image structs.
     */
    gtopoDEM ***DEMTiles = createTiles(inputDEM, factor);

    // Calculate the coordinates where each tile begins (top-left corner).
    point **readPoints = calculateReadPoints(inputDEM, DEMTiles, factor);

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
            for (subRow = 0; subRow < getHeight(DEMTiles[row][column]); subRow++)
            {
                for (subColumn = 0; subColumn < getWidth(DEMTiles[row][column]); subColumn++)
                {
                    // Assign current pixel to corresponding tile.
                    setElevation(DEMTiles[row][column], 
                        getElevation(inputDEM, readPoints[row][column].heightCoord + subRow,
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

    return DEMTiles;
}


/*
 * Adds the child DEM to the parent DEM with the top-left corner of the DEM
 * placed at the specified row and column values. Returns 0 on success and 1 on
 * failure if elevation points of a sub-DEM are placed outside of the larger DEM.
 */
int addDEM(gtopoDEM *parent, gtopoDEM *child, int startRow, int startColumn)
{
    int parentRow;
    int parentColumn;
    int childRow = 0;
    int childColumn = 0;
    int pointsWritten = 0;

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

            setElevation(parent, getElevation(child, childRow, childColumn), parentRow, parentColumn);
            childColumn++;
            pointsWritten++;
        }

        childRow++;
        childColumn = 0;
    }

    // Check that all pixels of the child/sub-image were written.
    if (pointsWritten != getWidth(child) * getHeight(child))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
