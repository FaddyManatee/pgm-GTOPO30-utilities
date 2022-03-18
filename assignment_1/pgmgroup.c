#include <stdio.h>
#include <stdlib.h>
#include "pgmdata.h"

typedef struct point
{
    int widthCoord;
    int heightCoord;
} point;


static pgmImage** createTiles(pgmImage *image, int factor)
{
    /*
     * Image tiles will be stored in an array and stored sequentially. The order
     * of the tiles in this array will be for a factor of 3: row0col0, row0col1,
     * row0col2, row1col0, row1col1, row1col2, row2col0, row2col1, row2col2.
     */
    // Calculate the width of right-most tiles.
    int tileRightWidth = (getWidth(image) / factor) + (getWidth(image) % factor);
    
    // Calculate the height of bottom-most tiles.
    int tileBottomHeight = (getHeight(image) / factor) + (getHeight(image) % factor);

    // Calculate width and height of pixels that aren't at on the very right or bottom.
    int tileWidth = (getWidth(image) / factor);
    int tileHeight = (getHeight(image) / factor);

    // Create nxn image structs.
    pgmImage **tiles = (pgmImage **) malloc(sizeof(pgmImage *) * factor * factor);
    int x;
    for (x = 0; x < factor * factor; x++)
    {
        tiles[x] = createImage();
    }
    
    // Initialise the tiles and set their dimensions.
    int row;
    int column;
    int tileNumber = 0;
    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            if (tileNumber < factor * factor)
            {
                if (row < factor - 1 && column < factor - 1)
                {
                    setDimensions(tiles[tileNumber], tileWidth, tileHeight);
                }
                else if (row < factor - 1 && column == factor - 1)
                {
                    setDimensions(tiles[tileNumber], tileRightWidth, tileHeight);
                }
                else if (row == factor - 1 && column < factor - 1)
                {
                    setDimensions(tiles[tileNumber], tileWidth, tileBottomHeight);
                }
                else if (row == factor - 1 && column == factor - 1)
                {
                    setDimensions(tiles[tileNumber], tileRightWidth, tileBottomHeight);
                }

                // Initialise the data of the tile.
                setMagicNumber(tiles[tileNumber], getMagicNumber(image), determineFormat(image));
                setMaxGrayValue(tiles[tileNumber], getMaxGrayValue(image));
                initImageRaster(tiles[tileNumber]);
                tileNumber++;
            }
        }
    }
    
    return tiles;
}


static point* calculateReadPoints(pgmImage *image, pgmImage **tiles, int factor)
{
    point *points = (point *) malloc(sizeof(point) * factor * factor);

    int row;
    int column;
    int tileNumber = 0;
    int currentWidth = 0;
    int currentHeight = 0;

    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {   
            points[tileNumber].widthCoord = currentWidth;
            points[tileNumber].heightCoord = currentHeight;

            currentWidth = currentWidth + getWidth(tiles[tileNumber]);
            tileNumber++;

            if (tileNumber == factor * factor)
                return points;
        }
        currentWidth = 0;
        currentHeight = currentHeight + getHeight(tiles[tileNumber]);
    }
}


pgmImage** tile(pgmImage *image, int factor)
{
    /* 
     * We need to split the input image into (factor * factor) smaller images. We
     * need an array of (factor * factor) image structs.
     */
    pgmImage **imageTiles = createTiles(image, factor);

    point *readPoints = calculateReadPoints(image, imageTiles, factor);

    int row;
    int column;
    int subRow;
    int subColumn;
    int tileNumber = 0;
    int tilePixel = 0;

    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            for (subRow = 0; subRow < getHeight(imageTiles[tileNumber]); subRow++)
            {
                for (subColumn = 0; subColumn < getWidth(imageTiles[tileNumber]); subColumn++)
                {
                    setPixel(imageTiles[tileNumber], getPixel(image, readPoints[tileNumber].heightCoord + subRow, readPoints[tileNumber].widthCoord + subColumn), tilePixel);
                    tilePixel++;
                }
            }
            tilePixel = 0;
            tileNumber++;

            if (tileNumber == factor * factor)
            {
                free(readPoints);
                return imageTiles;
            }
        }
    }
}
