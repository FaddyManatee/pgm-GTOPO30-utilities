#include "pgmio.h"

pgmImage*** tile(pgmImage *image, int factor);
void addImage(pgmImage *parent, pgmImage *child, int startRow, int startColumn);
