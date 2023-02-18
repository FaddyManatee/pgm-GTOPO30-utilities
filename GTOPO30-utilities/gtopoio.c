#include <stdio.h>
#include <ctype.h>
#include "gtopodata.h"
#include "gtopolimits.h"
#include "gtopoerror.h"

// Used to signal file errors to the programs that include this module.
gtopoError *error = NULL;


/*
 * Converts a signed short in big endian to little endian and vice versa.
 */
static signed short switchEndianness(signed short value)
{
    signed short switchedValue = (value << 8) | ((value >> 8) & 0xFF);
    return switchedValue;
}


/*
 * Reads the DEM raster, interpreting it as raw byte data. Can return an error.
 */
static void readRaster(gtopoDEM *inputDEM, FILE *file, char *path)
{
    int row;
    int column;
    int scanCount = 0;
    int pointsRead = 0;
    signed short elevation = 0;

    // Skip preceeding whitespace.
    fscanf(file, " ");

    // Start reading the binary raster data.
    for (row = 0; row < getHeight(inputDEM); row++)
    {   
        for (column = 0; column < getWidth(inputDEM); column++)
        {
            scanCount = fread(&elevation, sizeof(signed short), 1, file);

            // Since the read value was in big endian, we need to convert it to little endian.
            elevation = switchEndianness(elevation);

            // Check that the requested number of bytes was read.
            error = checkEOF(scanCount, path);
            if (error != NULL)
                return;

            // Check that the pixel we read is within valid range.
            error = checkElevation(elevation, scanCount, path);
            if (error != NULL)
                return;

            // Set the value if check passes.
            setElevation(inputDEM, elevation, row, column);
            pointsRead++;
        }
    }

    // Check whether the file contains more data than expected.
    elevation = NO_DATA - 1;
    while (fread(&elevation, sizeof(signed short), 1, file) != 0)
    {
        if (elevation != NO_DATA - 1)
            pointsRead++;
    }

    // Check that the number of elevation points read matched the dimensions.
    error = checkElevationCount(pointsRead, getWidth(inputDEM) * getHeight(inputDEM), path);
}


/*
 * Opens the file in read binary mode, and reads the DEM data. Returns NULL if
 * read failed.
 */
gtopoDEM* readDEM(char *filePath, int width, int height)
{
    error = NULL;

    // Open a file for reading.
    FILE *inputFile = fopen(filePath, "rb");
    
    // Check that the file path exists.
    error = checkInvalidFileName(inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Initialise the image.
    gtopoDEM *newDEM = createDEM(width, height);

    // Check that the image was allocated memory correctly.
    error = checkDEMallocated(newDEM);
    if (error != NULL)
        goto cleanup;

    // Read raster data, and check if an error occurred.
    readRaster(newDEM, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // We are finished with the file, tidy up.
    goto cleanup;

    cleanup:
    if (inputFile != NULL)
        fclose(inputFile);
    
    if (error != NULL)
        return NULL;
    
    return newDEM;
}


/*
 * Writes the image raster to a file in raw byte format.
 */
static void writeRaster(gtopoDEM *inputDEM, FILE *file)
{
    int width = getWidth(inputDEM);
    int height = getHeight(inputDEM);

    signed short elevation = 0;
    int row;
    int column;
    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column++)
        {
            elevation = getElevation(inputDEM, row, column);
            
            // DEM files store bytes in big-endian, we need to covert the little endian value.
            elevation = switchEndianness(elevation);

            fwrite(&elevation, sizeof(signed short), 1, file);
        }
    }
}


/*
 * Writes an image to disk given an image pointer and the file path, with the 
 * same raster data formatting as the original image. Can return an error.
 */
void echoDEM(gtopoDEM *inputDEM, char *filePath)
{
    error = NULL;

    FILE *outputFile = fopen(filePath, "wb");

    // Check that the file path exists.
    error = checkInvalidFileName(outputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Write the file if checks pass.
    writeRaster(inputDEM, outputFile);

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    if (outputFile != NULL)
        fclose(outputFile);
}
