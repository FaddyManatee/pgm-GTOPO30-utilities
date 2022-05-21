#include <stdlib.h>
#include "pgmlimits.h"

// Enable this #define directive for testing memory allocation failure.
// #define malloc(...) NULL


// http://netpbm.sourceforge.net/doc/pgm.html 

/*
 * Stores data related to comments. Properties of a comment include:
 * 
 * What line number they appear on, with indexing beginning at 0. 
 * Whether it exists on this line or not (1 for exists, 0 otherwise).
 * It's related string/buffer, with maximum length of MAX_COMMENT_LINE_LENGTH.
 * 
 * Other information: 
 * 
 * Comments may appear on seperate lines or at the end of lines.
 * Comments only appear in the header section of a pgm file, that is the lines
 * or seperate lines up until the image raster data.
 * Comments are prefixed with '#'
 */
typedef struct comment
{
    int lineNumber;
    int exists;
    char *commentString;
} comment;


/*
 * Stores all required data related to a pgm image that is to be handled by a
 * program. Properties of a pgm image include:
 * 
 * Raw: If the magic number is P5, the image raster data is stored in raw byte
 * format, whereas a magic number of P2 indicates the image raster data is stored
 * in ASCII character encoding. 0 indicates this data is encoded with ASCII and
 * 1 indicates it is stored in raw bytes.
 * 
 * Magic number: A sequence of two characters appearing at the top of a pgm file.
 * This will be stored internally as a sequence of two bytes to avoid issues with
 * endianness.
 * 
 * Width and height: Appear below the magic number and specifies the width and
 * height of the image in pixels. These may be split into two lines. 
 * 
 * Maximum Gray value: Appears below the width and height. It is greater than 0,
 * and less than 65536. If this value is less than or equal to 255, we need one
 * byte to store each pixel in the raster. Values higher than this are not
 * permitted.
 * 
 * Raster: Each pixel in the raster represents a gray value greater than 0 and less than 
 * the specified maximum gray value, which itself must be less than 255.
 * Every pixel uses the one byte.
 * 
 * Comments: Comments may appear in plainext pgm files (P2). They are implemented
 * using the defined "comment" data type. An arbitrary amount of comments are to
 * be stored.
 */
typedef struct image
{
    int format;
    int width; 
    int height;
    int maxGrayValue;
    unsigned short magicNumber;
    unsigned char **raster;
    comment *comments;
} image;


/*
 * Dynamically allocates memory to an image structure and returns a pointer to
 * the allocated memory so that its fields can be initialised and allocated
 * memory as required.
 */
image* createImage()
{
    // Allocate memory to a new image and set initial NULL/empty values. 
    image *newImage = (image *) malloc(sizeof(image));

    // Check for failed memory allocation and return NULL if this occurred.
    if (newImage == NULL)
        return NULL;

    newImage->format = DEFAULT_VALUE;
    newImage->width = DEFAULT_VALUE;
    newImage->height = DEFAULT_VALUE;
    newImage->maxGrayValue = DEFAULT_VALUE;
    newImage->magicNumber = 0;
    newImage->raster = NULL;
    
    // Allocate memory for storing comment lines and set initial NULL/empty values.
    newImage->comments = (comment *) malloc(sizeof(comment) * MAX_COMMENTS);

    // Check for failed memory allocation and return NULL if this occurred.
    if (newImage->comments == NULL)
        return NULL;

    int x;
    for (x = 0; x < MAX_COMMENTS; x++)
    {
        newImage->comments[x].commentString = (char *) calloc(MAX_COMMENT_LINE_LENGTH, sizeof(char));

        // Check for failed memory allocation and return NULL if this occurred.
        if (newImage->comments[x].commentString == NULL)
            return NULL;

        newImage->comments[x].exists = 0;
        newImage->comments[x].lineNumber = 0;
    }

    return newImage;
}


/*
 * Dynamically allocates enough memory to the image raster to store each pixel and
 * initialises their values to 0.
 */
void initImageRaster(image *image)
{       
    /* Allocate a 2D array to represent the raster. The 2D array contains arrays
     * and their total is the height of the image. Each of these arrays have
     * length equal to the width of the image.
     */

    image->raster = (unsigned char **) malloc(sizeof(unsigned char *) * image->height);

    // Allocate memory to each pixel in the raster and set them to an initial value of 0.
    int row;
    int column;

    for (row = 0; row < image->height; row++)
    {
        image->raster[row] = (unsigned char *) malloc(sizeof(unsigned char) * image->width);
    }

    for (row = 0; row < image->height; row++)
    {
        for (column = 0; column < image->width; column++)
        {
            image->raster[row][column] = 0;
        }
    }
}


/*
 *  Creates an empty image with the parameters given.
 */
image* createEmptyImage(int imageWidth, int imageHeight, int maxGray, int rawOrAscii)
{
    // Allocate memory to a new image and set the values.
    image *newImage = (image *) malloc(sizeof(image));

    //Check for failed memory allocation.
    if (newImage == NULL)
        return NULL;

    newImage->format = rawOrAscii;
    newImage->width = imageWidth;
    newImage->height = imageHeight;
    newImage->maxGrayValue = maxGray;
    initImageRaster(newImage);

    if (rawOrAscii == ASCII)
    {
        newImage->magicNumber = MAGIC_NUMBER_ASCII_PGM;
    }
    else if (rawOrAscii == RAW)
    {
        newImage->magicNumber = MAGIC_NUMBER_RAW_PGM;
    }

    // Allocate memory for storing comment lines and set initial NULL/empty values.
    newImage->comments = (comment *) malloc(sizeof(comment) * MAX_COMMENTS);

    if (newImage->comments == NULL)
        return NULL;

    int x;
    for (x = 0; x < MAX_COMMENTS; x++)
    {
        newImage->comments[x].commentString = (char *) calloc(MAX_COMMENT_LINE_LENGTH, sizeof(char));

        if (newImage->comments[x].commentString == NULL)
            return NULL;

        newImage->comments[x].exists = 0;
        newImage->comments[x].lineNumber = 0;
    }

    return newImage;
}


/*
 * Returns the value that determines whether the raster is in ASCII or raw byte
 * format for an image given its pointer, where 1 denotes raw and 0 for ASCII.
 */
int determineFormat(image *image)
{
    return image->format;
}

/*
 * Returns the comment that has the specified unique line number.
 */
char* getComment(image *image, int lineNo)
{
    int x;
    for (x = 0; x < MAX_COMMENTS; x++)
    {
        if (image->comments[x].lineNumber == lineNo)
        {
            return image->comments[x].commentString;
        }
    }

    // If no comment with the specified line number was found, return NULL.
    return NULL;
}

/*
 * Returns existance value of a comment at the specified line number.
 */
int getCommentExists(image *image, int lineNo)
{
    int x;
    for (x = 0; x < MAX_COMMENTS; x++)
    {
        if (image->comments[x].lineNumber == lineNo)
        {
            return image->comments[x].exists;
        }
    }
    return 0;
}


unsigned short getMagicNumber(image *image)
{
    return image->magicNumber;
}


int getWidth(image *image)
{
    return image->width;
}


int getHeight(image *image)
{
    return image->height;
}


int getMaxGrayValue(image *image)
{
    return image->maxGrayValue;
}


unsigned char** getRaster(image *image)
{
    return image->raster;
}


/*
 * Returns the value of the pixel in the raster given a pointer to the image and
 * the row and column this pixel should come from.
 */
unsigned char getPixel(image *image, int row, int column)
{
    return image->raster[row][column];
}

/*
 * Returns the address of an empty comment string that the comment should be read to.
 * Returns NULL if an empty comment string could not be found.
 */
char* setComment(image *image, int lineNo)
{
    int x;
    for (x = 0; x < MAX_COMMENTS; x++)
    {
        if (image->comments[x].exists == 1)
        {
            // A comment string already exists at this address, continue.
            continue;
        }

        /* 
         * Flag indexed comment as existing, set its line nunmber, and return the
         * memory address to store it in. 
         */
        image->comments[x].exists = 1;
        image->comments[x].lineNumber = lineNo;
        return image->comments[x].commentString;
    }

    // No free comment string. Return NULL.
    return NULL;
}

/*
 * Sets the magic number for an image given its pointer.
 */
void setMagicNumber(image *image, unsigned short magicNo, int rawOrAscii)
{
    image->format = rawOrAscii;
    image->magicNumber = magicNo;
}


/*
 * Sets the dimensions for an image given its pointer.
 */
void setDimensions(image *image, int width, int height)
{
    image->width = width;
    image->height = height;
}


/*
 * Sets the magic number for an image given its pointer.
 */
void setMaxGrayValue(image *image, int maxGray)
{
    image->maxGrayValue = maxGray;
}


/*
 *
 */
void setPixel(image *image, unsigned char value, int row, int column)
{
    image->raster[row][column] = value;
}


/*
 * Frees all dynamically allocated data related to an image given its pointer
 * which includes the raster and comments.
 */
void freeImage(image* image)
{
    // Only free if the image is initialised.
    if (image != NULL)
    {
        // Free memory allocated to comments.
        int x;

        for (x = 0; x < MAX_COMMENTS; x++)
        {
            free(image->comments[x].commentString);
        }
        free(image->comments);

        // Free memory allocated to the image raster if it was allocated.
        if (image->raster != NULL)
        {
            for (x = 0; x < image->height; x++)
            {
                free(image->raster[x]);
            }
            free(image->raster);
        }

        free(image);
    }
}
