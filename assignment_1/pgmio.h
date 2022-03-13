#include "pgmdata.h"
#include "pgmerror.h"
#include "pgmexit.h"

extern pgmError *error;

pgmImage* readImage(char *filePath);
void writeImage(pgmImage *image, char *filePath);
