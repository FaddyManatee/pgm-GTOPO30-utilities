typedef struct pgmErr pgmError;

pgmError* checkInvalidFileName(FILE *file, char *path);
pgmError* checkInvalidWriteMode(int mode);
pgmError* checkImageCanBeWritten(pgmImage *image, char *path);
pgmError* checkEOF(FILE *file);
pgmError* checkBinaryEOF(int scanned);
pgmError* checkComment(char *comment, char *path);
pgmError* checkCommentLimit(char *comment);
pgmError* checkInvalidMagicNo(unsigned short *magicNo, char *path);
pgmError* checkInvalidDimensions(unsigned int width, unsigned int height, int scanned, char *path);
pgmError* checkInvalidMaxGrayValue(unsigned int maxGray, int scanned, char *path);
pgmError* checkImageAllocated(pgmImage *image);
pgmError* checkRequiredData(pgmImage *image);
pgmError* checkRasterAllocated(pgmImage *image);
pgmError* checkPixel(unsigned int pixel, unsigned int maxGray, int scanned, char *path);
pgmError* checkPixelCount(int count, unsigned int expected, char *path);
int displayError(pgmError *err);
