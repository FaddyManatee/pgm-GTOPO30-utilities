#include <stdio.h>
#include <string.h>
#include "gtopoio.h"

// DEM (Digital Elevation Model)

/*
 * Prints the DEM raster data to the console using the following key:
 * 
 * ' ' (space): Sea (i.e. value <= sea)
 * '.' (full stop): Low ground (sea < value <= hill)
 * '^' (caret): Hills (hill < value <= mountain)
 * 'A': Mountains (mountain < value)
 */
void printLand(gtopoDEM *inputDEM, char *filePath, int sea, int hill, int mountain)
{
    // Open an ASCII file for writing.
    FILE *outputFile = fopen(filePath, "w");

    // Check that the file opened successfully.
    error = checkInvalidFileName(outputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Get the DEM raster data.
    signed short **raster = getRaster(inputDEM);

    /* Print the land using the key based on the elevation values of the
     * raster and user inputted elevation values for sea, hill and mountain to the
     * file.
     */
    int row;
    int column;
    signed short elevation = 0;

    for (row = 0; row < getHeight(inputDEM); row++)
    {
        for (column = 0; column < getWidth(inputDEM); column++)
        {
            elevation = getElevation(inputDEM, row, column);

            if (elevation <= sea)
            {
                fputc(' ', outputFile);
            }
            else if (elevation <= hill && elevation > sea)
            {
                fputc('.', outputFile);
            }
            else if (elevation <= mountain && elevation > hill)
            {
                fputc('^', outputFile);
            }
            else if (elevation > mountain)
            {
                fputc('A', outputFile);
            }
        }

        // Append a new line character to the end of each row.
        fputc('\n', outputFile);
    }

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    if (outputFile != NULL)
        fclose(outputFile);

    return;
}


int main(int argc, char **argv)
{
    /*
     * Check argument count is exactly equal to 8. The program requires only 8
     * arguments to be provided:
     * 
     * argv[0] = Program name
     * argv[1] = Input file path
     * argv[2] = Width of the DEM data 
     * argv[3] = Height of the DEM data
     * argv[4] = Output file path
     * argv[5] = Sea value
     * argv[6] = Hill value
     * argv[7] = Mountain value
     */
    if (argc == 1)
    {
        printf("Usage: %s inputFile width height outputFile sea hill mountain\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    else if (argc != 8)
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
     * Convert the sea CLI argument to an integer. Check that the hill value is valid.
     */
    char *sea;
    int seaDEM = strtol(argv[5], &sea, 10);


    /* 
     * Convert the hill CLI argument to an integer. Check that the hill value is valid.
     */
    char *hill;
    int hillDEM = strtol(argv[6], &hill, 10);


    /* 
     * Convert the mountain CLI argument to an integer. Check that the hill value is valid.
     */
    char *mountain;
    int mountainDEM = strtol(argv[7], &mountain, 10);


    // Check that the sea, hill and mountain elevation settings are valid.
    error = checkElevationSettings(seaDEM, hillDEM, mountainDEM, *sea, *hill, *mountain);
    if (error != NULL)
        return displayError(error);

    // Read DEM and store returned pointer to the elevation structure. 
    gtopoDEM *inputDEM = readDEM(argv[1], widthDEM, heightDEM);

    // If the external error pointer is no longer null, a file read error has been detected.
    if (error != NULL)
    {
        if (inputDEM != NULL)
            freeDEM(inputDEM);

        return displayError(error);
    }

    // Write the data to the output file in argv[4] using the symbols/keys.
    printLand(inputDEM, argv[4], seaDEM, hillDEM, mountainDEM);

    // Check if an error occurred attempting to write the file.
    if (error != NULL)
    {
        if (inputDEM != NULL)
            freeDEM(inputDEM);

        return displayError(error);
    }

    // Exit the program.
    freeDEM(inputDEM);
    return EXIT_NO_ERRORS;
}
