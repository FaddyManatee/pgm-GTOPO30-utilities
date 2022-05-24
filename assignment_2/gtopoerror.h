#include <stdio.h>

typedef struct gtopoErr gtopoError;

gtopoError* checkInvalidFileName(FILE *file, char *path);
gtopoError* checkInvalidFactor(int factor, char lastChar);
gtopoError* checkInvalidWidth(int width, char lastChar);
gtopoError* checkInvalidHeight(int height, char lastChar);
gtopoError* checkInvalidPosition(int axisPosition, int axisEnd, char lastChar);
gtopoError* checkTagsPresent(char *template, char *rowTag, char *colTag);
gtopoError* checkEOF(int scanned, char *path);
gtopoError* checkDEMallocated(gtopoDEM *targetDEM);
gtopoError* checkElevation(signed short elevation, int scanned, char *path);
gtopoError* checkElevationCount(int count, int expected, char *path);
gtopoError* checkElevationSettings(int sea, int hill, int mountain,
                char lastCharSea, char lastCharHill, char lastCharMountain);
int displayError(gtopoError *err);
