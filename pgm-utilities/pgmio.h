#include "pgmdata.h"
#include "pgmlimits.h"
#include "pgmerror.h"
#include "pgmexit.h"

extern pgmError *error;

pgmImage* readImage(char *filePath);
void echoImage(pgmImage *image, char *filePath);
void convert(pgmImage *image, char *filePath, int binaryOrAscii);
