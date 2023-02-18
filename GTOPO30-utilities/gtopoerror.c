#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtopodata.h"
#include "gtopolimits.h"
#include "gtopoexit.h"


/*
 * The data related to an error. Its integer error code and its associated string
 * displayed to the user to describe what went wrong. Data of this type is
 * initialised whenever an error is detected.
 */
typedef struct gtopoErr
{
    int errorCode;
    char *errorMsg;
} gtopoErr;


/*
 *
 */
static void createError(gtopoErr *err, int code, char *prefix , char *string)
{
    // Allocate enough memory to the error string and build it.
    err->errorMsg = (char *) calloc(strlen(prefix) + strlen(string) + 10, sizeof(char));
    err->errorCode = code;
    strcat(err->errorMsg, prefix);
    strcat(err->errorMsg, " ");

    if (strlen(string) > 0)
        strcat(err->errorMsg, "(");

    strcat(err->errorMsg, string);

    if (strlen(string) > 0)
        strcat(err->errorMsg, ")");
        
    strcat(err->errorMsg, "\n");
}


/*
 * Checks whenever a file stream failed to open.
 */
gtopoErr* checkInvalidFileName(FILE *file, char *path)
{
    gtopoErr *invalidFileName = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (file == NULL)
    {
        // We will free the error when we display it.
        createError(invalidFileName, EXIT_BAD_FILE_NAME, STR_BAD_FILE_NAME, path);
        return invalidFileName;
    }

    free(invalidFileName);
    return NULL;
}


/*
 * Checks whether the argument for factor is greater than 0.
 */
gtopoErr* checkInvalidFactor(int factor, char lastChar)
{
    gtopoErr *invalidFactor = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (factor <= 0 || lastChar != '\0')
    {
        createError(invalidFactor, EXIT_MISC, STR_MISC, STR_BAD_FACTOR);
        return invalidFactor;
    }

    free(invalidFactor);
    return NULL;
}


/*
 * Checks whether the argument for the width of a DEM is correct.
 */
gtopoErr* checkInvalidWidth(int width, char lastChar)
{
    gtopoErr *invalidWidth = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (width < MIN_DIMENSION || width > MAX_COLUMNS || lastChar != '\0')
    {
        createError(invalidWidth, EXIT_MISC, STR_MISC, STR_BAD_DIMENSION);
        return invalidWidth;
    }

    free(invalidWidth);
    return NULL;
}


/*
 * Checks whether the argument for the height of a DEM is correct.
 */
gtopoErr* checkInvalidHeight(int height, char lastChar)
{
    gtopoErr *invalidHeight = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (height < MIN_DIMENSION || height > MAX_ROWS || lastChar != '\0')
    {
        createError(invalidHeight, EXIT_MISC, STR_MISC, STR_BAD_DIMENSION);
        return invalidHeight;
    }

    free(invalidHeight);
    return NULL;
}


/*
 * Checks whether a row/column position for GTOPO assembly is valid
 */
gtopoErr* checkInvalidPosition(int axisPosition, int axisEnd, char lastChar)
{
    gtopoErr *invalidPosition = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (axisPosition < MIN_DIMENSION - 1 || axisPosition > axisEnd - 1 || lastChar != '\0')
    {
        createError(invalidPosition, EXIT_MISC, STR_MISC, STR_BAD_ROW);
        return invalidPosition;
    }

    free(invalidPosition);
    return NULL;
}


/*
 * Checks whether <row> and <column> tags are present in the template output file
 * names for pgmTile.
 */
gtopoErr* checkTagsPresent(char *template, char *rowTag, char *colTag)
{
    gtopoErr *tagMissing = (gtopoErr *) malloc(sizeof(gtopoErr));

    char *rowTagAddress = strstr(template, rowTag);
    char *columnTagAddress = strstr(template, colTag);

    if (rowTagAddress == NULL && columnTagAddress == NULL)
    {
        createError(tagMissing, EXIT_MISC, STR_MISC, STR_NO_TAGS);
        return tagMissing;
    }
    else if (rowTagAddress == NULL)
    {
        createError(tagMissing, EXIT_MISC, STR_MISC, STR_NO_ROW_TAG);
        return tagMissing;
    }
    else if (columnTagAddress == NULL)
    {
        createError(tagMissing, EXIT_MISC, STR_MISC, STR_NO_COL_TAG);
        return tagMissing;
    }

    free(tagMissing);
    return NULL;
}


/*
 *
 */
gtopoErr* checkEOF(int scanned, char *path)
{
    gtopoErr *eofDetected = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (scanned == 0)
    {
        // We will free the error when we display it.
        createError(eofDetected, EXIT_BAD_DATA, STR_BAD_DATA, path);
        return eofDetected;
    }
    
    free(eofDetected);
    return NULL;
}


/*
 *
 */
gtopoErr* checkDEMallocated(gtopoDEM *targetDEM)
{
    gtopoErr *notAllocated = (gtopoErr *) malloc(sizeof(gtopoErr));
    signed short **raster = getRaster(targetDEM);

    if (targetDEM == NULL || raster == NULL)
    {
        // We will free the error when we display it.
        createError(notAllocated, EXIT_MALLOC_FAILED, STR_MALLOC_FAILED, "");
        return notAllocated;
    }

    int row;
    for (row = 0; row < getHeight(targetDEM); row++)
    {
        if (raster[row] == NULL)
        {
            createError(notAllocated, EXIT_MALLOC_FAILED, STR_MALLOC_FAILED, "");
            return notAllocated;
        }
    }
    
    // Error not triggered. Free it.
    free(notAllocated);
    return NULL;
}


/*
 *
 */
gtopoErr* checkElevation(signed short elevation, int scanned, char *path)
{
    gtopoErr *badElevation = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (elevation != NO_DATA && (scanned != 1 || elevation > MAX_ELEVATION_VALUE || elevation < MIN_ELEVATION_VALUE))
    {
        // We will free this error when we display it.
        createError(badElevation, EXIT_BAD_DATA, STR_BAD_DATA, path);
        return badElevation;
    }

    // Error not triggered. Free it.
    free(badElevation);
    return NULL;
}


/*
 *
 */
gtopoErr* checkElevationCount(int count, int expected, char *path)
{
    gtopoErr *missingElevation = (gtopoErr *) malloc(sizeof(gtopoErr));

    if (count != expected)
    {
        // We will free this error when we display it.
        createError(missingElevation, EXIT_BAD_DATA, STR_BAD_DATA, path);
        return missingElevation;
    }

    // Error not triggered. Free it.
    free(missingElevation);
    return NULL;
}


/*
 * Checks whether the input elavation settings for sea, hill and mountain conform
 * to the specification.
 * 
 * ' ' (space): Sea (i.e. value <= sea)
 * '.' (full stop): Low ground (sea < value <= hill)
 * '^' (caret): Hills (hill < value <= mountain)
 * 'A': Mountains (mountain < value)
 */ 
gtopoErr* checkElevationSettings(int sea, int hill, int mountain,
                char lastCharSea, char lastCharHill, char lastCharMountain
)
{
    gtopoErr *invalidSettings = (gtopoErr *) malloc(sizeof(gtopoErr));

    // Check the values were read from the command line correctly.
    if (lastCharSea != '\0' || lastCharHill != '\0' || lastCharMountain != '\0')
    {
        // We will free this error when we display it.
        createError(invalidSettings, EXIT_MISC, STR_MISC, STR_BAD_SETTINGS);
        return invalidSettings;
    }

    // Any two of the three values should not equal each other.
    if (sea == hill || sea == mountain || hill == mountain)
    {
        // We will free this error when we display it.
        createError(invalidSettings, EXIT_MISC, STR_MISC, STR_BAD_SETTINGS);
        return invalidSettings;
    }

    // Mountain should be greater than hill and hill should be greater than sea.
    if (sea > hill || sea > mountain || hill > mountain)
    {
        // We will free this error when we display it.
        createError(invalidSettings, EXIT_MISC, STR_MISC, STR_BAD_SETTINGS);
        return invalidSettings;
    }

    // Check the elevation values are in the valid ranges with special case of -9999.
    if ((sea != NO_DATA && (sea < MIN_ELEVATION_VALUE || sea > MAX_ELEVATION_VALUE)) ||
        (hill != NO_DATA && (hill < MIN_ELEVATION_VALUE || hill > MAX_ELEVATION_VALUE)) ||
        (mountain != NO_DATA && (mountain < MIN_ELEVATION_VALUE || mountain > MAX_ELEVATION_VALUE)))
    {
        // We will free this error when we display it.
        createError(invalidSettings, EXIT_MISC, STR_MISC, STR_BAD_SETTINGS);
        return invalidSettings;
    }

    // Error not triggered. Free it.
    free(invalidSettings);
    return NULL;
}


/*
 * Displays the occurrance of an error to the user, printing the error string
 * and returning the exit code that should be used to exit the program with.
 */
int displayError(gtopoErr *err)
{
    printf(err->errorMsg);
    int code = err->errorCode;
    free(err->errorMsg);
    free(err);
    err = NULL;
    return code;
}
