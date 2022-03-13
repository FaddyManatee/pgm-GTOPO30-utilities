#include <stdio.h>
#include "pgmio.h"

// https://gabriellesc.github.io/teaching/resources/GDB-cheat-sheet.pdf

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 3. The program requires only 3
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Output file path
     */
    if (argc != 3)
    {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Read image file and store returned pointer to the image structure. 
    pgmImage *inputImage = readImage(argv[0]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        return displayError(error);
    }

    // Write the data referenced by the image pointer to a new file.
    writeFile(inputImage, argv[1]);

    // If the external error pointer is no longer null, a file write error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        return displayError(error);
    }

    // Display success string and exit the program
    printf(STR_ECHOED);
    return EXIT_NO_ERRORS;
}
