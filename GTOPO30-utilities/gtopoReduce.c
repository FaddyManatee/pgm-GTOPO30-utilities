#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "gtoposhrink.h"

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 6. The program requires only 6
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Width of the DEM data
     * argv[3] = Height of the DEM data
     * argv[4] = Integer factor
     * argv[5] = Output file path
     */
    if (argc == 1)
    {
        printf("Usage: %s input width height reduction_factor output\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    else if (argc != 6)
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

    /* 
     * Convert the factor CLI argument to an integer. Check that the factor is valid.
     * Has to be an integer greater than one.
     */
    char *end;
    int factor = strtol(argv[4], &end, 10);
    error = checkInvalidFactor(factor, *end);
    if (error != NULL)
        return displayError(error);

    // Read image file and store returned pointer to the image structure. 
    gtopoDEM *inputDEM = readDEM(argv[1], widthDEM, heightDEM);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputDEM != NULL)
            freeDEM(inputDEM);
            
        return displayError(error);
    }

    // If checks pass, reduce the image.
    gtopoDEM *reducedDEM = reduce(inputDEM, factor);

    // Write the data referenced by the reduced image pointer to a new file with same formatting.
    echoDEM(reducedDEM, argv[5]);

    // If the external error pointer is no longer null, a file write error has been detected.
    if (error != NULL)
    {
        freeDEM(inputDEM);
        freeDEM(reducedDEM);
        return displayError(error);
    }

    // Display success string and exit the program.
    freeDEM(inputDEM);
    freeDEM(reducedDEM);
    printf(STR_REDUCED);
    return EXIT_NO_ERRORS;
}
