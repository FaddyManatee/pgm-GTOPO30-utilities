#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmdata.h"
#include "pgmlimits.h"
#include "pgmexit.h"


/*
 * The data related to an error. Its integer error code and its associated string
 * displayed to the user to describe what went wrong. Data of this type is
 * initialised whenever an error is detected.
 */
typedef struct pgmErr
{
    int errorCode;
    char *errorMsg;
} pgmErr;


/*
 *
 */
static void createError(pgmErr *err, int code, char *prefix , char *string)
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
pgmErr* checkInvalidFileName(FILE *file, char *path)
{
    pgmErr *invalidFileName = (pgmErr *) malloc(sizeof(pgmErr));

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
pgmErr* checkInvalidFactor(int factor, char lastChar)
{
    pgmErr *invalidFactor = (pgmErr *) malloc(sizeof(pgmErr));

    if (factor <= 0 || lastChar != '\0')
    {
        createError(invalidFactor, EXIT_MISC, STR_MISC, STR_BAD_FACTOR);
        return invalidFactor;
    }

    free(invalidFactor);
    return NULL;
}


/*
 * Checks whether the argument for a dimension is greater than 0 and less than 65536.
 */
pgmErr* checkInvalidDimension(int dimension, char lastChar)
{
    pgmErr *invalidDimension = (pgmErr *) malloc(sizeof(pgmErr));

    if (dimension < MIN_IMAGE_DIMENSION || dimension > MAX_IMAGE_DIMENSION || lastChar != '\0')
    {
        createError(invalidDimension, EXIT_MISC, STR_MISC, STR_BAD_DIMENSION);
        return invalidDimension;
    }

    free(invalidDimension);
    return NULL;
}


/*
 * Checks whether a tuple for image assembly is non-empty and padded with parentheses.
 */
pgmErr* checkInvalidTupleFormat(char *tuple)
{
    pgmErr *invalidTuple = (pgmErr *) malloc(sizeof(pgmErr));

    if (!(strlen(tuple) > 0 && tuple[0] == '(' && tuple[strlen(tuple) - 1] == ')'))
    {
        createError(invalidTuple, EXIT_MISC, STR_MISC, STR_BAD_TUPLE);
        return invalidTuple;
    }

    free(invalidTuple);
    return NULL;
}


/*
 * Checks whether a tuple for image assembly contains 3 space delimited values.
 */
pgmErr* checkInvalidTupleElements(int count)
{
    pgmErr *invalidElements = (pgmErr *) malloc(sizeof(pgmErr));

    if (count != 3)
    {
        createError(invalidElements, EXIT_MISC, STR_MISC, STR_BAD_TUPLE);
        return invalidElements;
    }

    free(invalidElements);
    return NULL;
}


/*
 * Checks whether a tuple for image assembly contains a valid start row.
 */
pgmErr* checkInvalidRow(int row, int totalRows, char lastChar)
{
    pgmErr *invalidRow = (pgmErr *) malloc(sizeof(pgmErr));

    if (row < MIN_IMAGE_DIMENSION - 1 || row > totalRows - 1 || lastChar != '\0')
    {
        createError(invalidRow, EXIT_MISC, STR_MISC, STR_BAD_ROW);
        return invalidRow;
    }

    free(invalidRow);
    return NULL;
}


/*
 * Checks whether a tuple for image assembly contains a valid start column.
 */
pgmErr* checkInvalidColumn(int column, int totalColumns, char lastChar)
{
    pgmErr *invalidColumn = (pgmErr *) malloc(sizeof(pgmErr));

    if (column < MIN_IMAGE_DIMENSION - 1 || column > totalColumns - 1 || lastChar != '\0')
    {
        createError(invalidColumn, EXIT_MISC, STR_MISC, STR_BAD_COLUMN);
        return invalidColumn;
    }

    free(invalidColumn);
    return NULL;
}


/*
 * Checks whether <row> and <column> tags are present in the template output file
 * names for pgmTile.
 */
pgmErr* checkTagsPresent(char *template, char *rowTag, char *colTag)
{
    pgmErr *tagMissing = (pgmErr *) malloc(sizeof(pgmErr));

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
pgmErr* checkEOF(FILE *file, char *path)
{
    pgmErr *eofDetected = (pgmErr *) malloc(sizeof(pgmErr));

    if (feof(file))
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
pgmErr* checkBinaryEOF(int scanned, char *path)
{
    pgmErr *eofDetected = (pgmErr *) malloc(sizeof(pgmErr));

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
pgmErr* checkComment(char *comment, char *path)
{
    pgmErr *badCommentLine = (pgmErr *) malloc(sizeof(pgmErr));

    // Get last character. If not a new line, this is an invalid comment.
    // A valid comment of length n: comment[n] == '\0', comment[n - 1] == '\n'
    char lastChar = comment[strlen(comment) - 1];
    
    if (comment == NULL || lastChar != '\n')
    {
        // We will free the error when we display it.
        createError(badCommentLine, EXIT_BAD_COMMENT_LINE, STR_BAD_COMMENT_LINE, path);
        return badCommentLine;
    }
    
    free(badCommentLine);
    return NULL;
}


/*
 *
 */
pgmErr* checkCommentLimit(char *comment)
{
    pgmErr *commentLimitExceeded = (pgmErr *) malloc(sizeof(pgmErr));

    if (comment == NULL)
    {
        // We will free the error when we display it.
        createError(commentLimitExceeded, EXIT_MISC, STR_COMMENT_LIMIT, "");
        return commentLimitExceeded;
    }

    // Error not triggered. Free it.
    free(commentLimitExceeded);
    return NULL;
}


/*
 *
 */
pgmErr* checkInvalidMagicNo(unsigned short *magicNo, char *path)
{
    pgmErr *invalidMagicNo = (pgmErr *) malloc(sizeof(pgmErr));
    
    if ((*magicNo != MAGIC_NUMBER_ASCII_PGM) && (*magicNo != MAGIC_NUMBER_RAW_PGM))
    {
        // We will free the error when we display it.
        createError(invalidMagicNo, EXIT_BAD_MAGIC_NUMBER, STR_BAD_MAGIC_NUMBER, path);
        return invalidMagicNo;
    }

    // Error not triggered. Free it.
    free(invalidMagicNo);
    return NULL;
}


/*
 *
 */
pgmErr* checkInvalidDimensions(int width, int height, int scanned, char *path)
{
    pgmErr *invalidDimensions = (pgmErr *) malloc(sizeof(pgmErr));

    /* 
     * We expect fscanf to have scanned 2 integers, one for width and height.
     * We also expect the values for width and height fall within their valid range.
     */
    if (scanned != 2 ||
        width < MIN_IMAGE_DIMENSION || 
        width > MAX_IMAGE_DIMENSION ||
        height < MIN_IMAGE_DIMENSION ||
        height > MAX_IMAGE_DIMENSION)
    {
        // We will free the error when we display it.
        createError(invalidDimensions, EXIT_BAD_DIMENSIONS, STR_BAD_DIMENSIONS, path);
        return invalidDimensions;
    }

    // Error not triggered. Free it.
    free(invalidDimensions);
    return NULL;
}


/*
 *
 */
pgmErr* checkInvalidMaxGrayValue(int maxGray, int scanned, char *path)
{
    pgmErr *invalidMaxGrayValue = (pgmErr *) malloc(sizeof(pgmErr));

    /* 
     * We expect fscanf to have scanned 1 unsigned integer for maximum gray value.
     * We also expect the value for maximum gray value to fall within its valid range.
     */
    if (scanned != 1 || maxGray < MIN_GRAY_VALUE || maxGray > MAX_GRAY_VALUE)
    {
        // We will free the error when we display it.
        createError(invalidMaxGrayValue, EXIT_BAD_MAX_GRAY_VALUE, STR_BAD_MAX_GRAY_VALUE, path);
        return invalidMaxGrayValue;
    }

    // Error not triggered. Free it.
    free(invalidMaxGrayValue);
    return NULL;
}


/*
 *
 */
pgmErr* checkImageAllocated(pgmImage *image)
{
    pgmErr *imageNotAllocated = (pgmErr *) malloc(sizeof(pgmErr));

    if (image == NULL)
    {
        // We will free the error when we display it.
        createError(imageNotAllocated, EXIT_IMAGE_MALLOC_FAILED, STR_IMAGE_MALLOC_FAILED, "");
        return imageNotAllocated;
    }
    
    // Error not triggered. Free it.
    free(imageNotAllocated);
    return NULL;
}


/*
 *
 */
pgmErr* checkRequiredData(pgmImage *image, char *path)
{
    pgmErr *rasterNotAllocated = (pgmErr *) malloc(sizeof(pgmErr));

    if (getWidth(image) < MIN_IMAGE_DIMENSION ||
        getWidth(image) > MAX_IMAGE_DIMENSION ||
        getHeight(image) < MIN_IMAGE_DIMENSION ||
        getHeight(image) > MAX_IMAGE_DIMENSION ||
        getMaxGrayValue(image) < MIN_GRAY_VALUE ||
        getMaxGrayValue(image) > MAX_GRAY_VALUE)
    {
        // We will free the error when we display it.
        createError(rasterNotAllocated, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);
        return rasterNotAllocated;
    }

    // Error not triggered. Free it.
    free(rasterNotAllocated);
    return NULL;
}


/*
 *
 */
pgmErr* checkPixel(unsigned short pixel, int maxGray, int scanned, char *path)
{
    pgmErr *badPixel = (pgmErr *) malloc(sizeof(pgmErr));

    if (scanned != 1 || pixel > maxGray || pixel < MIN_PIXEL_VALUE || pixel > MAX_GRAY_VALUE)
    {
        // We will free this error when we display it.
        createError(badPixel, EXIT_BAD_DATA, STR_BAD_DATA, path);
        return badPixel;
    }

    // Error not triggered. Free it.
    free(badPixel);
    return NULL;
}


/*
 *
 */
pgmErr* checkPixelCount(int count, int expected, char *path)
{
    pgmErr *missingPixels = (pgmErr *) malloc(sizeof(pgmErr));

    if (count != expected)
    {
        // We will free this error when we display it.
        createError(missingPixels, EXIT_BAD_DATA, STR_BAD_DATA, path);
        return missingPixels;
    }

    // Error not triggered. Free it.
    free(missingPixels);
    return NULL;
}


/*
 *
 */
pgmErr* checkInvalidWriteMode(int mode)
{
    pgmErr *invalidWriteMode = (pgmErr *) malloc(sizeof(pgmErr));

    if (mode < 0 || mode > 1)
    {
        // We will free this error when we display it.
        createError(invalidWriteMode, EXIT_MISC, STR_BAD_WRITE_MODE, "");
        return invalidWriteMode;
    }

    // Error not triggered. Free it.
    free(invalidWriteMode);
    return NULL;
}


/*
 * 
 */
pgmErr* checkImageCanBeWritten(pgmImage *image, char *path)
{
    pgmErr *writeFailed = (pgmErr *) malloc(sizeof(pgmErr));

    if (image == NULL)
    {
        // We will free this error when we display it.
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);
        return writeFailed;
    }

    // If the image is allocated but not its raster, create an error.
    if (getRaster(image) == NULL)
    {
        // We will free this error when we display it.
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);
        return writeFailed;
    }

    // Check that the formatting, image dimensions, and maximum gray value are valid.
    if (determineFormat(image) < ASCII ||
        determineFormat(image) > RAW ||
        getWidth(image) < MIN_IMAGE_DIMENSION ||
        getWidth(image) > MAX_IMAGE_DIMENSION ||
        getHeight(image) < MIN_IMAGE_DIMENSION ||
        getHeight(image) > MAX_IMAGE_DIMENSION ||
        getMaxGrayValue(image) < MIN_GRAY_VALUE ||
        getMaxGrayValue(image) > MAX_GRAY_VALUE)
    {
        // We will free this error when we display it.
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);
        return writeFailed;
    }

    // Error not triggered. Free it.
    free(writeFailed);
    return NULL;
}


pgmErr* checkFileFormat(pgmImage *image, int convertFrom, char *path)
{
    pgmErr *badFileToConvert = (pgmErr *) malloc(sizeof(pgmErr));

    if (determineFormat(image) != convertFrom)
    {
        // We will free this error when we display it.
        createError(badFileToConvert, EXIT_BAD_MAGIC_NUMBER, STR_BAD_MAGIC_NUMBER, path);
        return badFileToConvert;
    }

    // Error not triggered. Free it.
    free(badFileToConvert);
    return NULL;
}


/*
 * Displays the occurrance of an error to the user, printing the error string
 * and returning the exit code that should be used to exit the program with.
 */
int displayError(pgmErr *err)
{
    printf(err->errorMsg);
    int code = err->errorCode;
    free(err->errorMsg);
    free(err);
    err = NULL;
    return code;
}
