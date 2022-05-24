#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "gtopogroup.h"
#include "gtoposhrink.h"

// Stores the data from the input tuples about the image and its placement.
typedef struct gtopoSubDEM
{
    gtopoDEM *subDEM;
    int startRow;
    int startColumn;
} gtopoSubDEM;


/*
 * Frees memory allocated to the sub-DEMs.
 */
void freeSubDEMs(gtopoSubDEM *subDEMs, int amount)
{
    int x;
    for (x = 0; x < amount; x++)
    {
        if (subDEMs[x].subDEM != NULL)
            freeDEM(subDEMs[x].subDEM);
    }
    free(subDEMs);
}


int main(int argc, char **argv)
{
    /*
     * Check argument count is greater than or equal to 10. The program requires 
     * at least 10 arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Output file name
     * argv[2] = Overall DEM width
     * argv[3] = Overall DEM height
     * argv[4] = Reduction Factor
     * 
     * argv[5] = row (tuple 1)
     * argv[6] = column 
     * argv[7] = Input file name
     * argv[8] = Width of this DEM data
     * argv[9] = Height of this DEM data (Minimum amount of arguments is here, argc == 10) 
     * 
     * argv[10] = row (tuple 2)
     * argv[11] = column
     * argv[12] = Input file name
     * argv[13] = Width of this DEM data
     * argv[14] = Height of this DEM data
     * 
     * ...
     */

    if (argc == 1)
    {
        printf("Usage: ././gtopoAssembleReduce outputArray.gtopo width height (row column inputArray.gtopo width height)+\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    
    // We expect a minimum of 10 arguments.
    if (argc < 10)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Check that arguments for images to be assembled are complete. We expect 5 per image.
    if ((argc - 5) % 5 != 0)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    /* 
     * Convert the width CLI argument to an integer. Check that the width is valid.
     * Has to be an integer greater than one.
     */
    char *width;
    int widthDEM = strtol(argv[2], &width, 10);

    error = checkInvalidWidth(widthDEM, *width);
    if (error != NULL)
        return displayError(error);

    /* 
     * Convert the height CLI argument to an integer. Check that the height is valid.
     * Has to be an integer greater than one.
     */
    char *height;
    int heightDEM = strtol(argv[3], &height, 10);

    error = checkInvalidHeight(heightDEM, *height);
    if (error != NULL)
        return displayError(error);

    /* 
     * Convert the factor CLI argument to an integer. Check that the factor is valid.
     * Has to be an integer greater than one.
     */
    char *factor;
    int factorDEM = strtol(argv[4], &factor, 10);

    error = checkInvalidFactor(factorDEM, *factor);
    if (error != NULL)
        return displayError(error);


    // Calculate the number of images to be assembled.
    int subDEMamount = (argc - 5) / 5;
    gtopoSubDEM *subDEMs = (gtopoSubDEM *) malloc(sizeof(gtopoSubDEM) * subDEMamount);

    if (error != NULL)
    {
        freeSubDEMs(subDEMs, subDEMamount);
        return displayError(error);
    }

    // Read tuple tupleData starting from argv[4] until we reach argc.
    int count;
    int argIndex = 5;
    for (count = 0; count < subDEMamount; count++) 
    {
        // Read the row location to insert the image at.
        char *row;
        int rowDEM = strtol(argv[argIndex], &row, 10);

        error = checkInvalidPosition(rowDEM, heightDEM, *row);
        if (error != NULL)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            return displayError(error);
        }

        // Store the sub-DEM's row starting position if no error occurred.
        subDEMs[count].startRow = rowDEM;


        // Read the column location to insert the image at.
        char *column;
        int columnDEM = strtol(argv[argIndex + 1], &column, 10);

        error = checkInvalidPosition(columnDEM, widthDEM, *column);
        if (error != NULL)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            return displayError(error);
        }

        // Store the sub-DEM's column starting position if no error occurred.
        subDEMs[count].startColumn = columnDEM;

        /* 
         * Convert the width CLI argument to an integer. Check that the width is valid.
         * Has to be an integer greater than one.
         */
        int subWidthDEM = strtol(argv[argIndex + 3], &width, 10);

        error = checkInvalidWidth(subWidthDEM, *width);
        if (error != NULL)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            return displayError(error);
        }

        /* 
         * Convert the height CLI argument to an integer. Check that the height is valid.
         * Has to be an integer greater than one.
         */
        int subHeightDEM = strtol(argv[argIndex + 4], &height, 10);

        error = checkInvalidHeight(subHeightDEM, *height);
        if (error != NULL)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            return displayError(error);
        }

        // Open the sub-DEM to be assembled.
        subDEMs[count].subDEM = readDEM(argv[argIndex + 2], subWidthDEM, subHeightDEM);
        
        // If the external error pointer is no longer null, a file read error has been detected.
        if (error != NULL)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            return displayError(error);
        }

        // Add 5 to argIndex to point to the next sub-DEM to insert.
        argIndex = argIndex + 5;
    }

    // Initialise the image that we assemble the sub-DEMs onto if no error occurred.
    gtopoDEM *parentDEM = createDEM(widthDEM, heightDEM);

    // Check that the image was allocated.
    error = checkDEMallocated(parentDEM);
    if (error != NULL)
    {
        freeDEM(parentDEM);
        freeSubDEMs(subDEMs, subDEMamount);
        return displayError(error);
    }

    // Add the sub-DEMs to the image.
    for (count = 0; count < subDEMamount; count++)
    {
        int success = addDEM(parentDEM, subDEMs[count].subDEM, subDEMs[count].startRow, 
                        subDEMs[count].startColumn);
        
        // If pixels to add were outside of the image, exit.
        if (success == 1)
        {
            freeSubDEMs(subDEMs, subDEMamount);
            freeDEM(parentDEM);
            printf(STR_BAD_LAYOUT);
            return EXIT_BAD_LAYOUT;
        }

    }

    // Reduce the assembled DEM data using the factor.
    gtopoDEM *reducedDEM = reduce(parentDEM, factorDEM);

    // Write the reduced final DEM data to disk with the path stored in argv[1].
    echoDEM(reducedDEM, argv[1]);

    // Check that the file wrote to disk properly.
    if (error != NULL)
    {
        freeSubDEMs(subDEMs, subDEMamount);
        freeDEM(parentDEM);
        freeDEM(reducedDEM);
        return displayError(error);
    }

    // Clean up before exiting.
    freeSubDEMs(subDEMs, subDEMamount);
    freeDEM(parentDEM);
    freeDEM(reducedDEM);

    // Display success string and exit the program.
    printf(STR_ASSEMBLED);
    return EXIT_NO_ERRORS;
}
