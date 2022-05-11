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
    int row = 0;
    int column = 0;

    // Find each and every space delimited value in the resulting string.
    for (token = strtok(tupleData, " "); token != NULL; token = strtok(NULL, " "))
    {
        if (tokenCount == 0)
        {
            // We expect the row to appear first.
            row = strtol(token, &rowToken, 10);
        }
        else if (tokenCount == 1)
        {
            // We expect the column to appear second.
            column = strtol(token, &columnToken, 10);
        }
        else if (tokenCount == 2)
        {
            // We expect the file path of the sub-image to appear last.
            subImage->path = (char *) malloc(sizeof(char) * strlen(token));
            strcpy(subImage->path, token);
        }

        tokenCount++;
    }

    // Check that the tuples contained the correct amount of data.
    error = checkInvalidTupleElements(tokenCount);
    if (error != NULL)
    {
        free(tupleData);
        return;
    }

    // Check that the starting row value is valid and is within the height bounds of the image.
    error = checkInvalidRow(row, totalHeight, *rowToken);
    if (error != NULL)
    {
        free(tupleData);
        return;
    }

    // If valid, set the starting row of the sub-image.
    subImage->startRow = row;


    // Check that the starting column value is valid and is within the width bounds of the image.
    error = checkInvalidColumn(column, totalWidth, *columnToken);
    if (error != NULL)
    {
        free(tupleData);
        return;
    }

    // If valid, set the starting column of the sub-image.
    subImage->startColumn = column;

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
     * 
     * Make sure that the argument in argv[1] is a path because the user can input 
     * the width, height and three image tuples, forgetting the output file name 
     * and get past the argument count check.
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
    int subImageAmount = argc - 4;
    pgmSubImage *subImages = (pgmSubImage *) malloc(sizeof(pgmSubImage) * subImageAmount);

    // Read tuple tupleData starting from argv[4] until we reach argc.
    int count;
    for (count = 0; count < subImageAmount; count++) 
    {
        // Tuples start from argv[4].
        char *tupleArgument = argv[count + 4];
        processTuple(&subImages[count], imageWidth, imageHeight, tupleArgument);

        // If the tuple is invalid, break.
        if (error != NULL)
            break;

        // Open the sub-image to be assembled.
        subImages[count].image = readImage(subImages[count].path);
        
        // If the external error pointer is no longer null, a file read error has been detected. Break.
        if (error != NULL)
            break;
    }

    // Display the error that occurred when processing the tuple data.
    if (error != NULL)
    {
        for (count = 0; count < subImageAmount; count++)
        {
            if (subImages[count].path != NULL)
                free(subImages[count].path);

            if (subImages[count].image != NULL)
                freeImage(subImages[count].image);
        }
        free(subImages);

        return displayError(error);
    }

    // If no error occurred, assemble the larger image from these sub-images.
    // Find the largest maximum gray value of the sub-images.
    int largestGray = MIN_GRAY_VALUE - 1;
    int currentGray = MIN_GRAY_VALUE - 1;
    for (count = 0; count < subImageAmount; count++)
    {
        currentGray = getMaxGrayValue(subImages[count].image);

        if (currentGray > largestGray)
            largestGray = currentGray;
    }

    // Initialise the image that we assemble the sub-images onto.
    pgmImage *image = createEmptyImage(imageWidth, imageHeight, largestGray, ASCII);

    // Add the sub-images to the image.
    for (count = 0; count < subImageAmount; count++)
    {
        addImage(image, subImages[count].image, subImages[count].startRow, subImages[count].startColumn);
    }

    // Write the final image to disk with the path stored in argv[1].
    echoImage(image, argv[1]);

    // Check that the file wrote to disk properly.
    if (error != NULL)
    {
        for (count = 0; count < subImageAmount; count++)
        {
            if (subImages[count].path != NULL)
                free(subImages[count].path);

            if (subImages[count].image != NULL)
                freeImage(subImages[count].image);
        }
        free(subImages);

        freeImage(image);

        return displayError(error);
    }

    // Display success string and exit the program.
    printf(STR_ASSEMBLED);
    return EXIT_NO_ERRORS;
}
