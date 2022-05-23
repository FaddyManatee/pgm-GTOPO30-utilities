#include <stdio.h>

// Includes pgmio.h. We can use pgm input/output functions and track the external error.
#include "pgmcompare.h"

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 3. The program requires only 3
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file 1 path
     * argv[2] = Input file 2 path
     */
    if (argc == 1)
    {
        printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    else if (argc != 3)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Read image file 1 and store returned pointer to the image structure. 
    pgmImage *inputImageOne = readImage(argv[1]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputImageOne != NULL)
            freeImage(inputImageOne);

        return displayError(error);
    }

    // Read image file 2 and store returned pointer to the image structure. 
    pgmImage *inputImageTwo = readImage(argv[2]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputImageOne != NULL)
            freeImage(inputImageOne);
        
        if (inputImageTwo != NULL)
            freeImage(inputImageTwo);

        return displayError(error);
    }

    // Compare the two images and determine logical equivalence.
    int result = compare(inputImageOne, inputImageTwo);

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
    freeImage(inputImageOne);
    freeImage(inputImageTwo);    
    return EXIT_NO_ERRORS;
}
