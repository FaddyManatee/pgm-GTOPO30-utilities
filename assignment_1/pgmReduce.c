#include <stdio.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmcrop.h"

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 3. The program requires only 3
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Integer factor
     * argv[3] = Output file path
     */
    if (argc != 4)
    {
        printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", argv[0]);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Check that the factor is valid. Has to be an integer.
    int factor = 0;
    error = checkInvalidFactor(scanf("%d", &factor));
    if (error != NULL)
        return displayError(error);

    // Read image file and store returned pointer to the image structure. 
    pgmImage *inputImage = readImage(argv[1]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        return displayError(error);
    }

    pgmImage *reducedImage = reduce(inputImage, factor);

    // If the external error pointer is no longer null, we couldn't reduce using the factor.
    if (error != NULL)
    {
        freeImage(inputImage);
        freeImage(reducedImage);
        return displayError(error);
    }

    // Write the data referenced by the reduced image pointer to a new file with same formatting.
    echoImage(reducedImage, argv[3]);

    // If the external error pointer is no longer null, a file write error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        freeImage(readImage);
        return displayError(error);
    }

    // Display success string and exit the program.
    freeImage(inputImage);
    freeImage(reducedImage);
    printf(STR_REDUCED);
    return EXIT_NO_ERRORS;
}
