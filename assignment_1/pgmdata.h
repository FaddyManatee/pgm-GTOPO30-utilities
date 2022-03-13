typedef struct comment pgmComment;
typedef struct image pgmImage;

pgmImage* createImage();
int determineFormat(pgmImage *image);
char* getComment(pgmImage *image, int lineNo);
int getCommentExists(pgmImage *image, int lineNo);
unsigned int getWidth(pgmImage *image);
unsigned int getHeight(pgmImage *image);
unsigned int getMaxGrayValue(pgmImage *image);
unsigned int getBytes(pgmImage *image);
unsigned short getPixel(pgmImage *image, int row, int column);
char* setComment(pgmImage *image, int lineNo);
void initImageRaster(pgmImage *image);
void setMagicNumber(pgmImage *image, unsigned short magicNo, int rawOrAscii);
void setDimensions(pgmImage *image, unsigned int width, unsigned int height);
void setMaxGrayValue(pgmImage *image, unsigned int maxGray);
void setPixel(pgmImage *image, unsigned int value, int pixelNo);
void freeImage(pgmImage *image);
