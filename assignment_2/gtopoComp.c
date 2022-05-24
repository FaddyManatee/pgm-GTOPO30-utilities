#include <stdio.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "gtopocompare.h"

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 5. The program requires only 5
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = First input file path
     * argv[2] = Width of the DEM data
     * argv[3] = Height of the DEM data
     * argv[4] = Second input file path
     */
    if (argc == 1)
    {
        printf("Usage: %s firstFile width height secondFile\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    else if (argc != 5)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }


    // Read width and height from argv[2] and argv[3] respectively.

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

    // Read DEM file 1 and store returned pointer to the DEM structure. 
    gtopoDEM *inputDEMOne = readDEM(argv[1], widthDEM, heightDEM);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputDEMOne != NULL)
            freeDEM(inputDEMOne);

        return displayError(error);
    }

    // Read DEM file 2 and store returned pointer to the DEM structure. 
    gtopoDEM *inputDEMTwo = readDEM(argv[4], widthDEM, heightDEM);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputDEMOne != NULL)
            freeDEM(inputDEMOne);
        
        if (inputDEMTwo != NULL)
            freeDEM(inputDEMTwo);

        return displayError(error);
    }

    // Compare the two images and determine logical equivalence.
    int result = compare(inputDEMOne, inputDEMTwo);

    // Display success string according to the result and exit the program.
    if (result == 1)
    {
        printf(STR_IDENTICAL);
    }
    else if (result == 0)
    {
        printf(STR_DIFFERENT);
    }

    // Free memory allocated to the images and exit.
    freeDEM(inputDEMOne);
    freeDEM(inputDEMTwo);    
    return EXIT_NO_ERRORS;
}
