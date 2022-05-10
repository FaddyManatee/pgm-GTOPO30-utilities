#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmgroup.h"

// Stores the data from the input tuples about the image and its placement.
typedef struct pgmSubImage
{
    pgmImage *image;
    char *path;
    int startRow;
    int startColumn;
} pgmSubImage;


/*
 * Extracts starting row, starting column and the file path from tuple arguments.
 * Can return an error.
 */
void processTuple(pgmSubImage *subImage, int totalWidth, int totalHeight, char *tuple)
{
    // Check that the tuple is valid.
    error = checkInvalidTupleFormat(tuple);
    if (error != NULL)
        return;

    // Remove the parentheses from the ends of the tuple.
    char *tupleData = (char *) malloc(sizeof(char) * strlen(tuple));

    int start;
    for (start = 1; start < strlen(tuple) - 1; start++)
    {
        tupleData[start - 1] = tuple[start];
    }

    char *token;
    char *rowToken;
    char *columnToken;
    char *pathToken;
    int tokenCount = 0;

    // Find each and every space delimited value in the resulting string.
    for (token = strtok(tupleData, " "); token != NULL; token = strtok(NULL, " "))
    {
        if (tokenCount == 0)
        {
            // We expect the row to appear first.
            int row = strtol(token, &rowToken, 10);

            // Check that the valueis valid and is within the height bounds of the image.
            error = checkInvalidRow(row, totalHeight, *rowToken);
            if (error != NULL)
            {
                free(tupleData);
                return;
            }

            // If valid, set the starting row of the sub image.
            subImage->startRow = row;
        }
        else if (tokenCount == 1)
        {
            // We expect the column to appear second.
            int column = strtol(token, &columnToken, 10);

            // Check that the value is valid and is within the width bounds of the image.
            error = checkInvalidColumn(column, totalWidth, *columnToken);
            if (error != NULL)
            {
                free(tupleData);
                return;
            }

            // If valid, set the starting column of the sub image.
            subImage->startColumn = column;
        }
        else if (tokenCount == 2)
        {
            // We expect the file path of the sub image to appear last.
            subImage->path = (char *) malloc(sizeof(char) * strlen(token));
            strcpy(subImage->path, token);
        }

        tokenCount++;
    }

    if (error != NULL)
    {
        free(tupleData);
        return;
    }

    free(tupleData);
}

int main(int argc, char **argv)
{
    /*
     * Check argument count is greater than or equal to 4. The program requires 
     * at least 4 arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Output file name
     * argv[2] = Image width
     * argv[3] = Image height
     * argv[4] = Tuple (row column inputImage.pgm)
     * argv[5] = ...
     * argv[6] = ...
     */
    if (argc == 1)
    {
        printf("Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    if (argc < 4)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    /* 
     * Convert the width CLI argument to an integer. Check that the width is valid.
     * Has to be an integer greater than one.
     */
    char *width;
    int imageWidth = strtol(argv[2], &width, 10);
    error = checkInvalidDimension(imageWidth, *width);
    if (error != NULL)
        return displayError(error);

    /* 
     * Convert the height CLI argument to an integer. Check that the height is valid.
     * Has to be an integer greater than one.
     */
    char *height;
    int imageHeight = strtol(argv[3], &height, 10);
    error = checkInvalidDimension(imageHeight, *height);
    if (error != NULL)
        return displayError(error);

    // Calculate the number of tuples/images to be assembled.
    int imageAmount = argc - 4;
    pgmSubImage *subImages = (pgmSubImage *) malloc(sizeof(pgmSubImage) * imageAmount);

    // Read tuple tupleData starting from argv[4] until we reach argc.
    int count;
    for (count = 0; count < imageAmount; count++) 
    {
        // Tuples start from argv[4].
        char *tupleArgument = argv[count + 4];
        processTuple(&subImages[count], imageWidth, imageHeight, tupleArgument);

        // If the tuple is invalid, exit the program.
        if (error != NULL)
            return displayError(error);

        // Open the sub image to be assembled.
        subImages[count].image = readImage(subImages[count].path);
        
        // If the external error pointer is no longer null, a file read error has been detected.
        if (error != NULL)
        {
            // freeImage(inputImage);
            // return displayError(error);
        }
    }

    // Display success string and exit the program.
    printf(STR_ASSEMBLED);
    return EXIT_NO_ERRORS;
}
