#include <stdio.h>
#include <stdlib.h>
#include "pgmdata.h"

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


pgmImage** tile(pgmImage *image, int factor)
{
    /* 
     * We need to split the input image into (factor * factor) smaller images. We
     * need an array of (factor * factor) image structs.
     */
    pgmImage **imageTiles = createTiles(image, factor);

    int row;
    int column;
    int subRow;
    int subColumn;
    int tileNumber = 0;
    int tilePixel = 0;
    int readFromRow = 0;
    int readFromColumn = 0;

    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            for (subRow = 0; subRow < getHeight(imageTiles[tileNumber]); subRow++)
            {
                for (subColumn = 0; subColumn < getWidth(imageTiles[tileNumber]); subColumn++)
                {
                    setPixel(imageTiles[tileNumber], getPixel(image, (row + 1) * subRow, (column + 1) * subColumn), tilePixel);
                }
            }

            readFromColumn = readFromColumn + (getHeight(image) / factor);
            tileNumber++;
            tilePixel = 0;

            if (tileNumber == factor * factor)
            {
                return imageTiles;
            }
        }
        readFromRow = readFromRow + (getWidth(image) / factor);
        readFromColumn = 0;
    }
}
