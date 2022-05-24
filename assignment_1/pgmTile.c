#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_TAG "<row>"
#define COL_TAG "<column>"

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmgroup.h"

void freeTiles(pgmImage ***tiles, int factor)
{
    int row;
    int column;

    for (row = 0; row < factor; row++)
    {
        free(tiles[row]);
    }
    free(tiles);
}


char* buildPath(char *format, int rowNumber, int columnNumber)
{
    // Counter variable.
    int x;

    // Copy the output file path format so that we can manipulate it.
    char *template = (char *) malloc(sizeof(char) * strlen(format) + 1);
    strcpy(template, format);

    // Convert the row number to a decimal representation
    char row[10];
    sprintf(row, "%d", rowNumber);

    // Convert the column number to a decimal representation.
    char column[10];
    sprintf(column, "%d", columnNumber);

    // Get starting address of <row> and <column>
    char *rowTagStart = strstr(template, ROW_TAG);
    char *columnTagStart = strstr(template, COL_TAG);

    // Set the memory of <row> to -1 to identify it for replacement.
    for (x = 0; x < strlen(ROW_TAG); x++)
    {
        rowTagStart[x] = '\377';
    }

    // Set the memory of <column> in the template to -2 to identify it for replacement.
    for (x = 0; x < strlen(COL_TAG); x++)
    {
        columnTagStart[x] = '\376';
    }

    /*
     * Allocate memory to the path string to return, zeroed by calloc to avoid corrupted
     * file names that are difficult to delete.
     */
    int pathLength = strlen(template) + strlen(row) + strlen(column) - (strlen(ROW_TAG) - strlen(COL_TAG));
    char *path = (char *) calloc(pathLength, sizeof(char));

    int rowWritten = 0;
    int columnWritten = 0;
    for (x = 0; x < strlen(template); x++)
    {
        char currentChar[2] = {template[x], '\0'};
        if (template[x] != '\377' && template[x] != '\376')
        {
            // The current character isn't part of a tag. Append the character.
            strcat(path, currentChar);
        }
        else if (template[x] == '\377' && rowWritten == 0)
        {
            // Current character is -1, flagging position of <row> tag. Append row number.
            strcat(path, row);
            rowWritten = 1;
        }
        else if (template[x] == '\376' && columnWritten == 0)
        {
            // Current character is -2, flagging position of <column> tag. Append column number.
            strcat(path, column);
            columnWritten = 1;
        }
    }

    free(template);
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
    if (argc == 1)
    {
        printf("Usage: %s inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    if (argc != 4)
    {
        printf(STR_BAD_ARGS_COUNT);
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
   error = checkTagsPresent(argv[3], ROW_TAG, COL_TAG);
   if (error != NULL)
        return displayError(error);

    // Read image file and store returned pointer to the image structure if checks pass.
    pgmImage *inputImage = readImage(argv[1]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputImage != NULL)
            freeImage(inputImage);
            
        return displayError(error);
    }

    // If checks pass, tile the image.
    pgmImage*** tiledImage = tile(inputImage, factor);

    int row;
    int column;

    for (row = 0; row < factor; row++)
    {
        for (column = 0; column < factor; column++)
        {
            // Write each image of the tile to disk.
            char *path = buildPath(argv[3], row, column);
            echoImage(tiledImage[row][column], path);

            // Check if an error occurred when writing the image.
            if (error != NULL)
            {
                freeImage(inputImage);
                freeTiles(tiledImage, factor);
                return displayError(error);
            }

            // Memory was allocated to path during buildPath(), free it.
            free(path);
        }
    }

    // Display success string and exit the program.
    freeImage(inputImage);
    freeTiles(tiledImage, factor);
    printf(STR_TILED);
    return EXIT_NO_ERRORS;
}
