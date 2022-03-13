#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmdata.h"
#include "pgmlimits.h"
#include "pgmexit.h"


/*
 *
 */
typedef struct pgmErr
{
    int errorCode;
    char *errorMsg;
} pgmErr;


/*
 *
 */
static pgmErr* createError(pgmErr *err, int code, char *prefix , char *string)
{
    err = (pgmErr *) malloc(sizeof(pgmErr));
    err->errorMsg = (char *) malloc(sizeof(char) * (strlen(prefix) + strlen(string) + 2));

    err->errorCode = code;
    strcat(err->errorMsg, prefix);
    strcat(err->errorMsg, " ");
    strcat(err->errorMsg, string); 
    
    return err;
}


/*
 *
 */
pgmErr* checkInvalidFileName(FILE *file, char *path)
{
    pgmErr *invalidFileName = NULL;

    if (file == NULL)
        createError(invalidFileName, EXIT_BAD_FILE_NAME, STR_BAD_FILE_NAME, path);

    return invalidFileName;
}


/*
 *
 */
pgmErr* checkEOF(FILE *file)
{
    pgmErr *eofDetected = NULL;

    if (feof(file))
        createError(eofDetected, EXIT_MISC, STR_MISC, STR_EOF);
    return eofDetected;
}


/*
 *
 */
pgmErr* checkBinaryEOF(int scanned)
{
    pgmErr *eofDetected = NULL;

    if (scanned == 0)
        createError(eofDetected, EXIT_MISC, STR_MISC, STR_EOF);
    return eofDetected;
}


/*
 *
 */
pgmErr* checkComment(char *comment, char *path)
{
    pgmErr *badCommentLine = NULL;

    if (comment == NULL)
        createError(badCommentLine, EXIT_BAD_COMMENT_LINE, STR_BAD_COMMENT_LINE, path);

    return badCommentLine;
}


/*
 *
 */
pgmErr* checkCommentLimit(char *comment)
{
    pgmErr *commentLimitExceeded = NULL;

    if (comment = NULL)
        createError(commentLimitExceeded, EXIT_MISC, STR_COMMENT_LIMIT, "");

    return commentLimitExceeded;
}


/*
 *
 */
pgmErr* checkInvalidMagicNo(unsigned short *magicNo, char *path)
{
    pgmErr *invalidMagicNo = NULL;
    
    if ((*magicNo != MAGIC_NUMBER_ASCII_PGM) || (*magicNo != MAGIC_NUMBER_RAW_PGM))
        createError(invalidMagicNo, EXIT_BAD_MAGIC_NUMBER, STR_BAD_MAGIC_NUMBER, path);

    return invalidMagicNo;
}


/*
 *
 */
pgmErr* checkInvalidDimensions(unsigned int width, unsigned int height, int scanned, char *path)
{
    pgmErr *invalidDimensions = NULL;

    /* 
     * We expect fscanf to have scanned 2 unsigned integers, one for width and height.
     * We also expect the values for width and height fall within their valid range.
     */
    if (scanned != 2 ||
        width < MIN_IMAGE_DIMENSION || 
        width > MAX_IMAGE_DIMENSION ||
        height < MIN_IMAGE_DIMENSION ||
        height > MAX_IMAGE_DIMENSION)
    {
        // Create an error
        createError(invalidDimensions, EXIT_BAD_DIMENSIONS, STR_BAD_DIMENSIONS, path);
    }
    return invalidDimensions;
}


/*
 *
 */
pgmErr* checkInvalidMaxGrayValue(unsigned int maxGray, int scanned, char *path)
{
    pgmErr *invalidMaxGrayValue = NULL;

    /* 
     * We expect fscanf to have scanned 1 unsigned integer for maximum gray value.
     * We also expect the value for maximum gray value to fall within its valid range.
     */
    if (scanned != 1 || maxGray < MIN_GRAY_VALUE || maxGray > MAX_GRAY_VALUE)
    {
        // Create an error
        createError(invalidMaxGrayValue, EXIT_BAD_MAX_GRAY_VALUE, STR_BAD_MAX_GRAY_VALUE, path);
    }
    return invalidMaxGrayValue;
}


/*
 *
 */
pgmErr* checkImageAllocated(pgmImage *image)
{
    pgmErr *imageNotAllocated = NULL;

    if (image == NULL)
        createError(imageNotAllocated, EXIT_IMAGE_MALLOC_FAILED, STR_IMAGE_MALLOC_FAILED, "");
    
    return imageNotAllocated;
}


/*
 *
 */
pgmErr* checkRequiredData(pgmImage *image)
{
    pgmErr *rasterNotAllocated = NULL;

    if (getWidth(image) < MIN_IMAGE_DIMENSION ||
        getWidth(image) > MAX_IMAGE_DIMENSION ||
        getHeight(image) < MIN_IMAGE_DIMENSION ||
        getHeight(image) > MAX_IMAGE_DIMENSION ||
        getMaxGrayValue(image) < MIN_GRAY_VALUE ||
        getMaxGrayValue(image) > MAX_GRAY_VALUE)
    {
        createError(rasterNotAllocated, EXIT_IMAGE_MALLOC_FAILED, STR_IMAGE_MALLOC_FAILED, "");
    }

    return rasterNotAllocated;
}


/*
 *
 */
pgmErr* checkRasterAllocated(pgmImage *image)
{
    pgmErr *rasterNotAllocated = NULL;

    if (getRaster(image) == NULL)
        createError(rasterNotAllocated, EXIT_IMAGE_MALLOC_FAILED, STR_IMAGE_MALLOC_FAILED, "");

    return rasterNotAllocated;
}


/*
 *
 */
pgmErr* checkPixel(unsigned int pixel, unsigned int maxGray, int scanned, char *path)
{
    pgmErr *badPixel = NULL;

    if (scanned != 1 || pixel > maxGray || pixel < MIN_GRAY_VALUE || pixel > MAX_GRAY_VALUE)
        createError(badPixel, EXIT_BAD_DATA, STR_BAD_DATA, path);

    return badPixel;
}


/*
 *
 */
pgmErr* checkPixelCount(int count, unsigned int expected, char *path)
{
    pgmErr *missingPixels = NULL;

    if (count != expected)
        createError(missingPixels, EXIT_BAD_DATA, STR_BAD_DATA, path);

    return missingPixels;
}


/*
 *
 */
pgmErr* checkInvalidWriteMode(int mode)
{
    pgmErr *invalidWriteMode = NULL;

    if (mode < 0 || mode > 1)
        createError(invalidWriteMode, EXIT_MISC, STR_BAD_WRITE_MODE, "");

    return invalidWriteMode;
}


/*
 *
 */
pgmErr* checkImageCanBeWritten(pgmImage *image, char *path)
{
    pgmErr *writeFailed = NULL;

    if (image == NULL)
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);

    // If the image is allocated but not its raster, create an error.
    if (writeFailed == NULL && getRaster(image) == NULL)
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);

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
        createError(writeFailed, EXIT_OUTPUT_FAILED, STR_OUTPUT_FAILED, path);
    }

    return writeFailed;
}


/*
 *
 */
static void freeError(pgmErr *err)
{
    free(err->errorMsg);
    free(err);
}


/*
 *
 */
int displayError(pgmErr *err)
{
    printf(err->errorMsg);
    int code = err->errorCode;
    freeError(err);
    err = NULL;
    return code;
}
