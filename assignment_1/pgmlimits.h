// http://netpbm.sourceforge.net/doc/pgm.html

/* 
 * Limits/constants common to both P2 and P5
 *
 * Both use ASCII decimal, however:
 * P5 is in raw binary format
 * P2 is in plain text
 */
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65535
#define MIN_GRAY_VALUE 1
#define MAX_GRAY_VALUE 65535
#define MAX_COMMENTS 128
#define MAX_COMMENT_LINE_LENGTH 128
#define DEFAULT_VALUE 0


/* 
 * Specific limits/constants for P5:
 *
 * Each pixel/gray value is represented in raw binary of size of 1 byte if
 * max gray value is less than 256.
 * 
 * Each pixel/gray value is represented in raw binary of size of 2 bytes if
 * max gray value is 256 or larger.
 * 
 * The most significant byte is first.
 */
#define MAGIC_NUMBER_RAW_PGM 0x3550


/*
 * Specific limits/constants for P2:
 *
 * Each pixel in the raster is represented as an ASCII decimal number (of arbitrary size).
 *
 * Each pixel in the raster has white space before and after it. There must be at least one
 * character of white space between any two pixels, but there is no maximum.
 * 
 * No line should be longer than 70 characters.
 */
#define MAGIC_NUMBER_ASCII_PGM 0x3250
