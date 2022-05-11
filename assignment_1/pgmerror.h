#include <stdio.h>

typedef struct pgmErr pgmError;

pgmError* checkInvalidFileName(FILE *file, char *path);
pgmError* checkInvalidWriteMode(int mode);
pgmError* checkInvalidFactor(int factor, char lastChar);
pgmError* checkInvalidDimension(int dimension, char lastChar);
pgmError* checkInvalidTupleFormat(char *tuple);
pgmError* checkInvalidTupleElements(int count);
pgmError* checkInvalidRow(int row, int totalRows, char lastChar);
pgmError* checkInvalidColumn(int column, int totalColumns, char lastChar);
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
pgmError* checkRequiredData(pgmImage *image, char *path);
pgmError* checkPixel(unsigned short pixel, int maxGray, int scanned, char *path);
pgmError* checkPixelCount(int count, int expected, char *path);
int displayError(pgmError *err);
