#include "pgmio.h"

pgmImage*** tile(pgmImage *image, int factor);
int addImage(pgmImage *parent, pgmImage *child, int startRow, int startColumn);
