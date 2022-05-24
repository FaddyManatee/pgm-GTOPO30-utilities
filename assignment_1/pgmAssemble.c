#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmgroup.h"

// Stores the data from the input tuples about the image and its placement.
typedef struct pgmSubImage
{
    pgmImage *image;
    int startRow;
    int startColumn;
} pgmSubImage;


/*
 * Frees memory allocated to the sub-images.
 */
void freeSubImages(pgmSubImage *images, int amount)
{
    int x;
    for (x = 0; x < amount; x++)
    {
        if (images[x].image != NULL)
            freeImage(images[x].image);
    }
    free(images);
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
     * 
     * argv[4] = row (tuple 1)
     * argv[5] = column 
     * argv[6] = Input file name (Minimum amount of arguments is here, argc == 7) 
     * 
     * argv[7] = row (tuple 2)
     * argv[8] = column
     * argv[9] = Input file name
     * 
     * ...
     */

    if (argc == 1)
    {
        printf("Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    
    // We expect a minimum of 7 arguments.
    if (argc < 7)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Check that arguments for images to be assembled are complete. We expect 3 per image.
    if ((argc - 4) % 3 != 0)
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

    error = checkInvalidDimensionSize(imageWidth, *width);
    if (error != NULL)
        return displayError(error);

    /* 
     * Convert the height CLI argument to an integer. Check that the height is valid.
     * Has to be an integer greater than one.
     */
    char *height;
    int imageHeight = strtol(argv[3], &height, 10);

    error = checkInvalidDimensionSize(imageHeight, *height);
    if (error != NULL)
        return displayError(error);


    // Calculate the number of images to be assembled.
    int subImageAmount = (argc - 4) / 3;
    pgmSubImage *subImages = (pgmSubImage *) malloc(sizeof(pgmSubImage) * subImageAmount);

    if (error != NULL)
    {
        freeSubImages(subImages, subImageAmount);
        return displayError(error);
    }

    // Read tuple tupleData starting from argv[4] until we reach argc.
    int count;
    int argIndex = 4;
    for (count = 0; count < subImageAmount; count++) 
    {
        // Read the row location to insert the image at.
        char *row;
        int imageRow = strtol(argv[argIndex], &row, 10);

        error = checkInvalidPosition(imageRow, imageHeight, *row);
        if (error != NULL)
        {
            freeSubImages(subImages, subImageAmount);
            return displayError(error);
        }

        // Store the sub-image's row starting position if no error occurred.
        subImages[count].startRow = imageRow;


        // Read the column location to insert the image at.
        char *column;
        int imageColumn = strtol(argv[argIndex + 1], &column, 10);

        error = checkInvalidPosition(imageColumn, imageWidth, *column);
        if (error != NULL)
        {
            freeSubImages(subImages, subImageAmount);
            return displayError(error);
        }

        // Store the sub-image's column starting position if no error occurred.
        subImages[count].startColumn = imageColumn;

        // Open the sub-image to be assembled.
        subImages[count].image = readImage(argv[argIndex + 2]);
        
        // If the external error pointer is no longer null, a file read error has been detected.
        if (error != NULL)
        {
            freeSubImages(subImages, subImageAmount);
            return displayError(error);
        }

        // Add 3 to argIndex to point to the next sub-image to insert.
        argIndex = argIndex + 3;
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

    // Check that the image was allocated.
    error = checkImageAllocated(image);

    if (error != NULL)
    {
        freeImage(image);
        freeSubImages(subImages, subImageAmount);
        return displayError(error);
    }

    // Add the sub-images to the image.
    for (count = 0; count < subImageAmount; count++)
    {
        int success = addImage(image, subImages[count].image, subImages[count].startRow, 
                        subImages[count].startColumn);
        
        // If pixels to add were outside of the image, exit.
        if (success == 1)
        {
            freeSubImages(subImages, subImageAmount);
            freeImage(image);
            printf(STR_BAD_LAYOUT);
            return EXIT_BAD_LAYOUT;
        }

    }

    // Write the final image to disk with the path stored in argv[1].
    echoImage(image, argv[1]);

    // Check that the file wrote to disk properly.
    if (error != NULL)
    {
        freeSubImages(subImages, subImageAmount);
        freeImage(image);
        return displayError(error);
    }

    // Display success string and exit the program.
    printf(STR_ASSEMBLED);
    return EXIT_NO_ERRORS;
}
