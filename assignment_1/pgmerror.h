#include <stdio.h>

typedef struct pgmErr pgmError;

pgmError* checkInvalidFileName(FILE *file, char *path);
pgmError* checkInvalidWriteMode(int mode);
pgmError* checkInvalidFactor(int factor, char lastChar);
pgmError* checkInvalidDimensionSize(int dimension, char lastChar);
pgmError* checkInvalidPosition(int axisPosition, int axisEnd, char lastChar);
pgmError* checkTagsPresent(char *template, char *rowTag, char *colTag);
pgmError* checkFileFormat(pgmImage *image, int convertFrom, char *path);
pgmError* checkImageCanBeWritten(pgmImage *image, char *path);
pgmError* checkEOF(FILE *file, char *path);
pgmError* checkBinaryEOF(int scanned, char *path);
pgmError* checkComment(char *comment, char *path);
pgmError* checkCommentLimit(char *comment);
pgmError* checkInvalidMagicNo(unsigned short *magicNo, char *path);
pgmError* checkInvalidDimensions(int width, int height, int scanned, char *path);
pgmError* checkInvalidMaxGrayValue(int maxGray, int scanned, char *path);
pgmError* checkImageAllocated(pgmImage *image);
pgmError* checkRasterAllocated(unsigned char **raster, int width, int height);
pgmError* checkRequiredData(pgmImage *image, char *path);
pgmError* checkPixel(unsigned char pixel, int maxGray, int scanned, char *path);
pgmError* checkPixelCount(int count, int expected, char *path);
int displayError(pgmError *err);
