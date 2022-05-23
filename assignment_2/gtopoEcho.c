#include <stdio.h>
#include "gtopoio.h"

// DEM (Digital Elevation Model)

int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 3. The program requires only 3
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Width of the DEM data 
     * argv[3] = Height of the DEM data
     * argv[4] = Output file path
     */
    if (argc == 1)
    {
        printf("Usage: %s inputFile width height outputFile\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    else if (argc != 5)
    {
        printf(STR_BAD_ARGS_COUNT);
        return EXIT_BAD_ARGS_COUNT;
    }

    // Read DEM and store returned pointer to the elevation structure. 
    gtopoData *inputElevation = readElevation(argv[1]);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputImage != NULL)
            freeElevation(inputImage);

        return displayError(error);
    }

    // Write the data referenced by the image pointer to a new file with same formatting.
    echoElevation(inputElevation, argv[4]);

    // If the external error pointer is no longer null, a file write error has been detected.
    if (error != NULL)
    {
        freeImage(inputImage);
        return displayError(error);
    }

    // Display success string and exit the program.
    freeImage(inputImage);
    printf(STR_ECHOED);
    return EXIT_NO_ERRORS;
}
