#include <stdio.h>

typedef struct pgmErr pgmError;

pgmError* checkInvalidFileName(FILE *file, char *path);
pgmError* checkInvalidWriteMode(int mode);
pgmError* checkInvalidFactor(int scanned, char lastChar);
pgmError* checkTagsPresent(char *rowTag, char *columnTag);
pgmError* checkImageCanBeWritten(pgmImage *image, char *path);
pgmError* checkEOF(FILE *file, char *path);
pgmError* checkBinaryEOF(int scanned, char *path);
pgmError* checkComment(char *comment, char *path);
pgmError* checkCommentLimit(char *comment);
pgmError* checkInvalidMagicNo(unsigned short *magicNo, char *path);
pgmError* checkInvalidDimensions(int width, int height, int scanned, char *path);
pgmError* checkInvalidMaxGrayValue(int maxGray, int scanned, char *path);
pgmError* checkImageAllocated(pgmImage *image);
pgmError* checkRequiredData(pgmImage *image);
pgmError* checkRasterAllocated(pgmImage *image);
pgmError* checkPixel(unsigned short pixel, int maxGray, int scanned, char *path);
pgmError* checkPixelCount(int count, int expected, char *path);
int displayError(pgmError *err);
