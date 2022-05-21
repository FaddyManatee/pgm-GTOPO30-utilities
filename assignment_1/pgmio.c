#include <stdio.h>
#include <ctype.h>
#include "pgmdata.h"
#include "pgmlimits.h"
#include "pgmerror.h"

// Used to signal file errors to the programs that include this module.
pgmError *error = NULL;

/*
 * Detects that a newline character exists at the end of a line. Can return an error.
 */
static void detectNewLine(FILE *file, int *line, char *path)
{
    char x = 0;
    while (x = fgetc(file))
    {
        if (x == '\n')
        {
            (*line)++;
            return;
        }
        else if (x != ' ')
        {
            ungetc(x, file);
            return;
        }
    }
}


/*
 * Checks for a comment line or sequential comment lines and reads them. Can
 * return an error.
 */
static void readComments(pgmImage *image, int *line, FILE *file, char *path)
{
    char nextChar = 0;
    do
    {
        // Skip any amount of whitespace before non-whitespace characters.
        fscanf(file, " ");

        // Get the next character from file to examine.
        nextChar = fgetc(file);
        if (nextChar == EOF)
            return;

        // Check that the character is a comment prefix.
        if (nextChar == '#')
        {
            // Get the address of an empty comment string/buffer.
            char *commentBuffer = setComment(image, *line);

            // Check if an address was available, if not we have run out of comments to store.
            error = checkCommentLimit(commentBuffer);
            if (error != NULL)
                return;
                
            // Read the comment to the end of the line.
            char *commentString = fgets(commentBuffer, MAX_COMMENT_LINE_LENGTH, file);

            // Check that the comment read was successful.
            error = checkComment(commentString, path);
            if (error != NULL)
                return;

            // Increment line number by one. We have read a comment line.
            (*line)++;
        }
        else
        {
            // Put the examined character back. Not a comment line.
            ungetc(nextChar, file);
            return;
        }
    } while (nextChar == '#');
}


/*
 * Reads the magic number of the image. Can return an error.
 */
static void readMagicNumber(pgmImage *image, int *line, FILE *file, char *path)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    char magicNumber[2] = {'0', '0'};

    // Skip any amount of whitespace before non-whitespace characters.
    fscanf(file, " ");
    // Stop reading if end of file is reached.
    error = checkEOF(file, path);
    if (error != NULL)
        return;

    // Read in the two magic number characters.
    magicNumber[0] = fgetc(file);
    error = checkEOF(file, path);
    if (error != NULL)
        return;

    magicNumber[1] = fgetc(file);
    error = checkEOF(file, path);
    if (error != NULL)
        return;
    
    // Check for a new line character
    detectNewLine(file, line, path);
    if (error != NULL)
        return;

    // Retrieve pointer to the two character bytes in memory of the magic number.
    unsigned short *magicNumberBytes = (unsigned short *) magicNumber;
    
    // Check that these bytes are valid.
    error = checkInvalidMagicNo(magicNumberBytes, path);
    if (error != NULL)
        return;

    // Increment line number by one. We have read a line.
    (*line)++;

    // Set these bytes and determine formatting of raster if check passes.
    if (*magicNumberBytes == MAGIC_NUMBER_RAW_PGM)
    {
        // 1 - P5 (Binary)
        setMagicNumber(image, *magicNumberBytes, RAW);
    }
    else if (*magicNumberBytes == MAGIC_NUMBER_ASCII_PGM)
    {
        // 0 - P2 (ASCII)
        setMagicNumber(image, *magicNumberBytes, ASCII);
    }
}


/*
 * Reads the width and height of the image. Can return an error.
 */
static void readDimensions(pgmImage *image, int *line, FILE *file, char *path)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    int width = 0;
    int height = 0;

    // Skip preceeding whitespace and read in width.
    int scanCount = fscanf(file, " %u", &width);
    error = checkEOF(file, path);
    if (error != NULL)
        return;

    // Check for a newline character.
    detectNewLine(file, line, path);
    if (error != NULL)
        return;
    
    // Skip preceeding whitespace and read in height.
    scanCount = scanCount + fscanf(file, " %u", &height);
    error = checkEOF(file, path);
    if (error != NULL)
        return;

    // Check for a newline character.
    detectNewLine(file, line, path);
    if (error != NULL)
        return;

    // Check that width and height are valid.
    error = checkInvalidDimensions(width, height, scanCount, path);
    if (error != NULL)
        return;

    // Set width and height if check passes.
    setDimensions(image, width, height);
}


/*
 * Reads maximum gray value. Can return an error.
 */
static void readMaxGrayValue(pgmImage *image, int *line, FILE *file, char *path)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    int maxGrayValue = 0;

    // Skip preceding whitespace and read in max gray value.
    int scanCount = fscanf(file, " %u", &maxGrayValue);
    error = checkEOF(file, path);
    if (error != NULL)
        return;

    // Check for a newline character.
    detectNewLine(file, line, path);
    if (error != NULL)
        return;

    // Check that the maximum gray value is valid.
    error = checkInvalidMaxGrayValue(maxGrayValue, scanCount, path);
    if (error != NULL)
        return;

    // Set the value if check passes.
    setMaxGrayValue(image, maxGrayValue);
}


/*
 * Reads the image raster, interpreting it as ASCII data. Can return an error.
 */
static void readAsciiData(pgmImage *image, FILE *file, char *path, int *line)
{
    int row;
    int column;
    int scanCount = 0;
    int pixelsRead = 0;
    int pixel = 0;

    // Start reading the ASCII raster data.
    for (row = 0; row < getHeight(image); row++)
    {
        for (column = 0; column < getWidth(image); column++)
        {
            readComments(image, line, file, path);
            if (error != NULL)
                return;

            error = checkEOF(file, path);
            if (error != NULL)
                return;

            if (getComment(image, *line) == NULL)
            {
                // Skip preceding whitespace and read in next pixel in raster.
                scanCount = fscanf(file, " %u", &pixel);

                // Check that the pixel we read is within valid range.
                error = checkPixel(pixel, getMaxGrayValue(image), scanCount, path);
                if (error != NULL)
                    return;

                // Set the value if check passes.
                // Integer value of pixel should be small enough to fit the short type.
                setPixel(image, pixel, row, column);

                if ((pixelsRead + 1) % getWidth(image) == 0)
                    (*line)++;

                pixelsRead++;
            }
        }
    }

    // Check that the file does not contain too much raster data.
    char character;
    while ((character = fgetc(file)) != EOF)
    {
        if (!isspace(character))
            pixelsRead++;
    }

    // Check that the number of pixels read matched the dimensions.
    error = checkPixelCount(pixelsRead, getWidth(image) * getHeight(image), path);
}


/*
 * Reads the image raster, interpreting it as raw byte data. Can return an error.
 */
static void readRawData(pgmImage *image, FILE *file, char *path)
{
    int row;
    int column;
    int scanCount = 0;
    int pixelsRead = 0;
    unsigned char pixel = 0;

    // Skip preceeding whitespace.
    fscanf(file, " ");

    // Start reading the binary raster data.
    for (row = 0; row < getHeight(image); row++)
    {   
        for (column = 0; column < getWidth(image); column++)
        {
            scanCount = fread(&pixel, 1, 1, file);

            // Check that the requested number of bytes was read.
            error = checkBinaryEOF(scanCount, path);
            if (error != NULL)
                return;

            // Check that the pixel we read is within valid range.
            error = checkPixel(pixel, getMaxGrayValue(image), scanCount, path);
            if (error != NULL)
                return;

            // Set the value if check passes.
            setPixel(image, pixel, row, column);
            pixelsRead++;
        }
    }

    // Check whether the file contains more data than expected.
    pixel = 0;
    while (fread(&pixel, 1, 1, file) != 0)
    {
        if (pixel > 0)
            pixelsRead++;
    }

    // Check that the number of pixels read matched the dimensions.
    error = checkPixelCount(pixelsRead, getWidth(image) * getHeight(image), path);
}


/*
 * Reads the image raster. Can return an error.
 */
static void readRaster(pgmImage *image, FILE *file, char *filePath, int *line)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    // Check that we have enough data to allocate memory to the image raster.
    error = checkRequiredData(image, filePath);
    if (error != NULL)
        return;

    /* 
     * We now have enough information about the image to allocate and initialise
     * the values of the allocated image's raster.
     */
    initImageRaster(image);

    // Double check raster was allocated properly.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    // Determine the type of data (binary or ASCII) the raster is stored in.
    int raw = determineFormat(image);

    // Choose appropriate read statements based on the type of data of the image raster.
    if (raw == 0)
    {
        readAsciiData(image, file, filePath, line);
    }
    else if (raw == 1)
    {
        readRawData(image, file, filePath);
    }
}


/*
 * Opens the file in read binary mode, and reads the image data. Data in the
 * header should be encoded in plaintext ASCII. The magic number is used to 
 * determine whether we need to interpret the raster data as bytes or ASCII.
 * Returns NULL if read failed.
 */
pgmImage* readImage(char *filePath)
{
    error = NULL;

    // Record line number so that we can track comment positions before raster data.
    int line;
    int *lineNumber;

    // Initial line number.
    line = 0;
    lineNumber = &line;

    // Open a file for reading.
    FILE *inputFile = fopen(filePath, "rb");
    
    // Check that the file path exists.
    error = checkInvalidFileName(inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Initialise the image.
    pgmImage *newImage = createImage();

    // Check that the image was allocated memory correctly.
    error = checkImageAllocated(newImage);
    if (newImage == NULL)
        goto cleanup;

    // Read comments that occur before the magic number
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read magic number and check if an error occurred.
    readMagicNumber(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read comments that occur before the dimensions.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read width and height, and check if an error occurred.
    readDimensions(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read comments that occur before the maximum gray value.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read maximum gray value, and check if an error occurred.
    readMaxGrayValue(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read comments that occur before the raster.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
        goto cleanup;

    // Read raster data, and check if an error occurred.
    readRaster(newImage, inputFile, filePath, lineNumber);
    if (error != NULL)
        goto cleanup;

    // We are finished with the file, tidy up.
    goto cleanup;

    cleanup:
    if (inputFile != NULL)
        fclose(inputFile);
    
    if (error != NULL)
        return NULL;
    
    return newImage;
}


/*
 * Writes consecutive comment lines that appeared in the read image.
 */
static void writeCommentLines(pgmImage *image, FILE *file, int *line)
{
    int count = 0;
    do
    {
        // Get the comment string that was read at the specified line. 
        char *comment = getComment(image, *line);

        // Write the comment to the file if not empty. Comments are already terminated with \n.
        if (comment != NULL)
        {
            if (comment[0] != '\0')
            {
                fputs("#", file);
                fputs(comment, file);
            }
        }

        (*line)++;
        // Loop while there exists a comment on the next line.
    } while (getCommentExists(image, *line) == 1);
}


/*
 * Writes the header section of a pgm image to the output file.
 */
static void writeHeader(pgmImage *image, FILE *file, int format, int *line)
{
    // Write comment lines that appear before magic number.
    writeCommentLines(image, file, line);

    // Write the magic number according to format.
    if (format == ASCII)
    {
        fputs("P2\n", file);
    }
    else if (format == RAW)
    {
        fputs("P5\n", file);
    }
    (*line)++;

    // Write comment lines that appear before dimensions.
    writeCommentLines(image, file, line);

    // Write the dimensions.
    fprintf(file, "%u %u\n", getWidth(image), getHeight(image));
    (*line)++;

    // Write comment lines that appear before maximum gray value.
    writeCommentLines(image, file, line);

    // Write the maximum gray value.
    fprintf(file, "%u\n", getMaxGrayValue(image));
    (*line)++;
}


/*
 * Writes the image raster to a file in ascii format.
 */
static void writeAsciiData(pgmImage *image, FILE *file, int *line)
{
    int width = getWidth(image);
    int height = getHeight(image);

    int row;
    int column;
    for (row = 0; row < height; row++)
    {
        writeCommentLines(image, file, line);
        for (column = 0; column < width; column++)
        {
            // For the first pixel or first pixel in a row, write without preceding whitespace.
            if ((row == 0 && column == 0) || column == 0)
            {
                fprintf(file, "%u", getPixel(image, row, column));
            }
            else
            {
                // Write other pixels padded with whitespace.
                fprintf(file, " %u", getPixel(image, row, column));
            }
        }

        // Append a newline character before moving onto the next row in raster.
        fputs("\n", file);
    }

    // Write comments that appear after the raster data.
    writeCommentLines(image, file, line);
}


/*
 * Writes the image raster to a file in raw byte format.
 */
static void writeBinaryData(pgmImage *image, FILE *file)
{
    int width = getWidth(image);
    int height = getHeight(image);

    unsigned char pixel = 0;
    int row;
    int column;
    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column++)
        {
            pixel = getPixel(image, row, column);
            fwrite(&pixel, 1, 1, file);
        }
    }
}


/*
 * Performs checks to determine whether it is safe to write to the output file.
 * Can return an error.
 */
static void imageWriteChecks(pgmImage *image, FILE *file, char *path, int choice)
{
    // Check that the file path exists.
    error = checkInvalidFileName(file, path);
    if (error != NULL)
        return;

    // Check that the specified output format is valid.
    error = checkInvalidWriteMode(choice);
    if (error != NULL)
        return;

    // Check if image has all required data to write to file.
    error = checkImageCanBeWritten(image, path);
    if (error != NULL)
        return;
}


/*
 * Writes an image to disk given an image pointer and the file path, with the 
 * same raster data formatting as the original image. Can return an error.
 */
void echoImage(pgmImage *image, char *filePath)
{
    error = NULL;

    // Record line number so that we can track comment positions before raster data.
    int line;
    int *lineNumber;

    // Initial line number.
    line = 0;
    lineNumber = &line;

    FILE *outputFile = fopen(filePath, "wb");

    // Perform checks to see if we can actually write the image.
    int formatting = determineFormat(image);
    imageWriteChecks(image, outputFile, filePath, formatting);
    if (error != NULL)
        goto cleanup;

    // Write the file "header" if checks pass.
    *lineNumber = 0;
    writeHeader(image, outputFile, formatting, lineNumber);

    // Write the raster in the format specified by binaryOrAscii.
    if (formatting == ASCII)
    {
        writeAsciiData(image, outputFile, lineNumber);
    }
    else if (formatting == RAW)
    {
        writeBinaryData(image, outputFile);
    }

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    if (outputFile != NULL)
        fclose(outputFile);
}


/*
 * Writes an image to disk given an image pointer, the file path, and if the
 * output file should be in ASCII or binary format. A value of 0 indicates that
 * the output file should be in ASCII format, whilst a value of 1 indicates that
 * the output file should be in binary format. Can return an error.
 */
void convert(pgmImage *image, char *filePath, int binaryOrAscii)
{
    error = NULL;

    // Record line number so that we can track comment positions before raster data.
    int line;
    int *lineNumber;

    // Initial line number.
    line = 0;
    lineNumber = &line;

    FILE *outputFile = fopen(filePath, "wb");

    // Perform checks to see if we can actually write the image.
    imageWriteChecks(image, outputFile, filePath, binaryOrAscii);
    if (error != NULL)
        goto cleanup;

    // Write the file "header" if checks pass.
    *lineNumber = 0;
    writeHeader(image, outputFile, binaryOrAscii, lineNumber);

    // Write the raster in the format specified by binaryOrAscii.
    if (binaryOrAscii == ASCII)
    {
        writeAsciiData(image, outputFile, lineNumber);
    }
    else if (binaryOrAscii == RAW)
    {
        writeBinaryData(image, outputFile);
    }

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    if (outputFile != NULL)
        fclose(outputFile);
}
