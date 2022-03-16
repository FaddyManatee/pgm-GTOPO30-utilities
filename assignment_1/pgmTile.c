#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmgroup.h"

void freeTiles(pgmImage **tiles, int factor)
{
    int x;
    for (x = 0; x < factor * factor; x++)
    {
        freeImage(tiles[x]);
    }
}

char* buildPath(char *template, int rowNumber, int columnNumber)
{
    char *row;
    sprintf(row, "%d", rowNumber);

    char *column;
    sprintf(column, "%d", columnNumber);

    // Get starting address of <row> and <column>
    char *rowTagStart = strstr(template, "row");
    char *columnTagStart = strstr(template, "column");

    // Set the memory of <row> to -1 to identify it.
    int x;
    for (x = 0; x < strlen("row"); x++)
    {
        rowTagStart[x] = -1;
    }

    // Set the memory of <column> in the template to -2 to identify it.
    for (x = 0; x < strlen("column"); x++)
    {
        rowTagStart[x] = -2;
    }

    int pathLength = strlen(template) + strlen(row) + strlen(column) - (strlen("row") - strlen("column"));
    char *path = (char *) malloc(sizeof(char) * pathLength);

    int rowWritten;
    int columnWritten;
    for (x = 0; x < strlen(template); x++)
    {
        if (template[x] != -1 || template[x] != -2)
        {
            strcat(path, &template[x]);
        }
        else if (template[x] == -1 && rowWritten == 0)
        {
            strcat(path, row);
            rowWritten = 1;
        }
        else if (template[x] == -2 && columnWritten == 0)
        {
            strcat(path, column);
            columnWritten = 1;
        }
    }

    return path;
}


int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 3. The program requires only 3
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Integer factor
     * argv[3] = Output file path template needed <row> and <column> tags
     */
    if (argc != 4)
    {
        printf("Usage: %s inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n", argv[0]);
        return EXIT_BAD_ARGS_COUNT;
    }

    /* 
     * Convert the factor CLI argument to an integer. Check that the factor is valid.
     * Has to be an integer greater than one.
     */
    char *end;
    int factor = strtol(argv[2], &end, 10);
    error = checkInvalidFactor(factor, *end);
    if (error != NULL)
        return displayError(error);

    /*
     * Check that the output file path template contains the <row> and <column> tags.
    */
   char *rowTagAddress = strstr(argv[3], "row");
   char *columnTagAddress = strstr(argv[3], "column");
   error = checkTagsPresent(rowTagAddress, columnTagAddress);
   if (error != NULL)
        return displayError(error);

    // Read image file and store returned pointer to the image structure if checks pass.
    pgmImage *inputImage = readImage(argv[1]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        return displayError(error);
    }

    // If checks pass, tile the image.
    pgmImage** tiledImage = tile(inputImage, factor);

    int row;
    int column;
    int tileNumber;
    for (tileNumber = 0; tileNumber < factor * factor; tileNumber++)
    {
        for (row = 0; row < factor; row++)
        {
            for (column = 0; column < factor; column++)
            {
                echoImage(tiledImage[tileNumber], buildPath(argv[3], row, column));
                if (error != NULL)
                {
                    free(inputImage);
                    freeTiles(tiledImage, factor);
                    return displayError(error);
                }
            }
        }
    }

    // Display success string and exit the program.
    freeImage(inputImage);
    freeTiles(tiledImage, factor);
    printf(STR_TILED);
    return EXIT_NO_ERRORS;
}
