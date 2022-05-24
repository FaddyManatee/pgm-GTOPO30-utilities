#include "gtopodata.h"
#include "gtopolimits.h"
#include "gtopoerror.h"
#include "gtopoexit.h"

extern gtopoError *error;

gtopoDEM* readDEM(char *filePath, int width, int height);
void echoDEM(gtopoDEM *inputFile, char *filePath);
