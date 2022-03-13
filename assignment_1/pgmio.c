#include <stdio.h>
#include "pgmdata.h"
#include "pgmlimits.h"
#include "pgmerror.h"

pgmError *error = NULL;


/*
 * Opens the file in read binary mode, and reads the image data. Data in the
 * header should be encoded in plaintext ASCII. The magic number is used to 
 * determine whether we need to interpret the raster data as bytes or ASCII.
 */
pgmImage* readImage(char *filePath)
{
    // Record line number so that we can track comment positions before raster data.
    int *lineNumber;
    error = NULL;

    pgmImage *newImage = createImage();

    // Initial line number;
    *lineNumber = 0;

    FILE *inputFile = fopen(filePath, "rb");

    // Check that the file path exists.
    error = checkInvalidFileName(inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read comments that occur before the magic number
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read magic number and check if an error occurred.
    readMagicNumber(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read comments that occur before the dimensions.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read width and height, and check if an error occurred.
    readDimensions(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read comments that occur before the maximum gray value.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read maximum gray value, and check if an error occurred.
    readMaxGrayValue(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read comments that occur before the raster.
    readComments(newImage, lineNumber, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // Read raster data, and check if an error occurred. We no longer need line number.
    readRaster(newImage, inputFile, filePath);
    if (error != NULL)
    {
        newImage = NULL;
        goto cleanup;
    }

    // We are finished with the file, tidy up.
    goto cleanup;

    cleanup:
    fclose(inputFile);
    return newImage;
}


/*
 * Checks for a comment line or sequential comment lines and reads them.
 */
static void readComments(pgmImage *image, int *line, FILE *file, char *filePath)
{
    char nextChar = 0;
    do
    {
        // Skip any amount of whitespace before non-whitespace characters.
        fscanf(file, " ");

        // Get the next character from file to examine.
        nextChar = fgetc(file);
        error = checkEOF(file);
        if (error != NULL)
            return;

        // Check that the character is a comment prefix.
        if (nextChar == '#')
        {
            // Put the examined character back.
            ungetc(nextChar, file);

            // Get the address of an empty comment string/buffer.
            char *commentBuffer = setComment(image, *line);

            // Check if an address was available, if not we have run out of comments to store.
            error = checkCommentLimit(commentBuffer);
            if (error != NULL)
                return;
                
            // Read the comment starting from '#' to the end of the line.
            char *commentString = fgets(commentBuffer, MAX_COMMENT_LINE_LENGTH, file);

            // Check that the comment read was successful.
            error = checkComment(commentString, filePath);
            if (error != NULL)
                return;

            // Increment line number by one. We have read a comment line.
            *line++;
            return;
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
 * Reads the magic number of the image.
 */
static void readMagicNumber(pgmImage *image, int *line, FILE *file, char *filePath)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    char magicNumber[2] = {'0', '0'};

    // Skip any amount of whitespace before non-whitespace characters.
    fscanf(file, " ");
    // Stop reading if end of file is reached.
    error = checkEOF(file);
    if (error != NULL)
        return;

    // Read in the two magic number characters.
    magicNumber[0] = fgetc(file);
    error = checkEOF(file);
    if (error != NULL)
        return;

    magicNumber[1] = fgetc(file);
    error = checkEOF(file);
    if (error != NULL)
        return;

    // Retrieve pointer to the two character bytes in memory of the magic number.
    unsigned short *magicNumberBytes = (unsigned short *) magicNumber;
    
    // Check that these bytes are valid.
    error = checkInvalidMagicNo(magicNumberBytes, filePath);
    if (error != NULL)
        return;

    // Increment line number by one. We have read a line.
    *line++;

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
 * Reads the width and height of the image.
 */
static void readDimensions(pgmImage *image, int *line, FILE *file, char *filePath)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    unsigned int width = 0;
    unsigned int height = 0;

    // Skip preceeding whitespace and read in width and height.
    int scanCount = fscanf(file, " %u %u", &width, &height);
    error = checkEOF(file);
    if (error != NULL)
        return;

    // Check that width and height are valid.
    error = checkInvalidDimensions(width, height, scanCount, filePath);
    if (error != NULL)
        return;

    // Set width and height if check passes.
    setDimensions(image, width, height);

    // Increment line number by one. We have read a line.
    *line++;
}


/*
 * Reads maximum gray value.
 */
static void readMaxGrayValue(pgmImage *image, int *line, FILE *file, char *filePath)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    unsigned int maxGrayValue = 0;

    // Skip preceding whitespace and read in max gray value.
    int scanCount = fscanf(file, " %u", &maxGrayValue);
    error = checkEOF(file);
    if (error != NULL)
        return;

    // Check that the maximum gray value is valid.
    error = checkInvalidMaxGrayValue(maxGrayValue, scanCount, filePath);
    if (error != NULL)
        return;

    // Set the value if check passes.
    setMaxGrayValue(image, maxGrayValue);

    // Increment line number by one. We have read a line.
    *line++;
}


/*
 * Reads the image raster.
 */
static void readRaster(pgmImage *image, FILE *file, char *filePath)
{
    // Check if image is allocated.
    error = checkImageAllocated(image);
    if (error != NULL)
        return;

    // Check that we have enough data to allocate memory to the image raster.
    error = checkRequiredData(image);
    if (error != NULL)
        return;

    /* 
     * We now have enough information about the image to allocate and initialise
     * the values of the allocated image's raster.
     */
    initImageRaster(image);

    // Double check raster was allocated properly.
    error = checkRasterAllocated(image);
    if (error != NULL)
        return;

    // Determine the type of data (binary or ASCII) the raster is stored in.
    int raw = determineFormat(image);

    // Choose appropriate read statements based on the type of data of the image raster.
    if (raw == 0)
    {
        readAsciiData(image, file, filePath);
    }
    else if (raw == 1)
    {
        readRawData(image, file, filePath);
    }
}


/*
 * Reads the image raster, interpreting it as ASCII data.
 */
static void readAsciiData(pgmImage *image, FILE *file, char *path)
{
    int x;
    int scanCount = 0;
    int pixelsRead = 0;
    unsigned int pixel = 0;

    // Start reading the ASCII raster data.
    for (x = 0; x < getWidth(image) * getHeight(image); x++)
    {
        // Skip preceding whitespace and read in next pixel in raster.
        scanCount = fscanf(file, " %u", &pixel);

        // Check if we tried to read beyond the last item in the file.
        error = checkEOF(file);
        if (error != NULL)
            return;

        // Check that the pixel we read is within valid range.
        error = checkPixel(pixel, getMaxGrayValue(image), scanCount, path);
        if (error != NULL)
            return;

        // Set the value if check passes.
        // Integer value of pixel should be small enough to fit the short type.
        setPixel(image, pixel, x);
        pixelsRead++;
    }

    // Check that the number of pixels read matched the dimensions.
    error = checkPixelCount(scanCount, getWidth(image) * getHeight(image), path);
}


/*
 * Reads the image raster, interpreting it as raw byte data.
 */
static void readRawData(pgmImage *image, FILE *file, char *path)
{
    int x;
    int scanCount = 0;
    int pixelsRead = 0;
    unsigned short pixel = 0;

    // Determine how many bytes a pixel uses. 
    int bytesNeeded = getBytes(image);

    // Skip preceeding whitespace.
    fscanf(file, " ");

    // Start reading the binary raster data.
    for (x = 0; x < getWidth(image) * getHeight(image); x++)
    {
        // ENDIANNESS?
        scanCount = fread(&pixel, bytesNeeded, 1, file);

        // Check that the requested number of bytes was read.
        error = checkBinaryEOF(scanCount);
        if (error != NULL)
            return;

        // Check that the pixel we read is within valid range.
        error = checkPixel(pixel, getMaxGrayValue(image), scanCount, path);
        if (error != NULL)
            return;

        // Set the value if check passes.
        setPixel(image, pixel, x);
        scanCount++;
    }

    // Check that the number of pixels read matched the dimensions.
    error = checkPixelCount(scanCount, getWidth(image) * getHeight(image), path);
}


/*
 * Writes an image to disk given an image pointer and the file path, with the 
 * same raster data formatting as the original image.
 */
void echoImage(pgmImage *image, char *filePath)
{
    int *lineNumber;
    error = NULL;

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
        writeAsciiData(image, outputFile);
    }
    else if (formatting == RAW)
    {
        writeBinaryData(image, outputFile);
    }

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    fclose(outputFile);
}


/*
 * Writes an image to disk given an image pointer, the file path, and if the
 * output file should be in ASCII or binary format. A value of 0 indicates that
 * the output file should be in ASCII format, whilst a value of 1 indicates that
 * the output file should be in binary format.
 */
void writeImage(pgmImage *image, char *filePath, int binaryOrAscii)
{
    int *lineNumber;
    error = NULL;

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
        writeAsciiData(image, outputFile);
    }
    else if (binaryOrAscii == RAW)
    {
        writeBinaryData(image, outputFile);
    }

    // We are now done with the file. Close it.
    goto cleanup;

    cleanup:
    fclose(outputFile);
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
    *line++;

    // Write comment lines that appear before dimensions.
    writeCommentLines(image, file, line);

    // Write the dimensions.
    fprintf(file, "%u %u\n", getWidth(image), getHeight(image));
    *line++;

    // Write comment lines that appear before maximum gray value.
    writeCommentLines(image, file, line);

    // Write the maximum gray value.
    fprintf(file, "%u\n", getMaxGrayValue(image));
    *line++;

    // Write comment lines that appear before the raster. Line number no longer needed.
    writeCommentLines(image, file, line);
}


/*
 * Writes the image raster to a file in ascii format.
 */
static void writeAsciiData(pgmImage *image, FILE *file)
{
    int width = getWidth(image);
    int height = getHeight(image);

    int x;
    int y;
    for (x = 0; x < height; x++)
    {
        for (y = 0; y < width; y++)
        {
            // For the first pixel, write without preceding whitespace.
            if (x == 0 && y == 0)
                fprintf(file, "%u", getPixel(image, x, y));

            // Write remaining pixels padded with whitespace.
            fprintf(file, " %u", getPixel(image, x, y));
        }

        // Append a newline character before moving onto the next row in raster.
        fputc("\n", file);
    }
}


/*
 * Writes the image raster to a file in raw byte format.
 */
static void writeBinaryData(pgmImage *image, FILE *file)
{
    int width = getWidth(image);
    int height = getHeight(image);

    int bytesNeeded = getBytes(image);
    unsigned short pixel = 0;

    int x;
    int y;
    for (x = 0; x < height; x++)
    {
        for (y = 0; y < width; y++)
        {
            // ENDIANNESS?
            pixel = getPixel(image, x, y);
            fwrite(&pixel, bytesNeeded, 1, file);
        }

        // Append a newline character before moving onto the next row in raster.
        fputc("\n", file);
    }
}

/*
 * Performs checks to determine whether it is safe to write to the output file.
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
}

/*
 * Writes consecutive comment lines that appeared in the read image.
 */
static void writeCommentLines(pgmImage *image, FILE *file, int *line)
{
    int count = 0;
    do
    {
        // Avoid incrementing line number on first iteration.
        if (count > 0)
            *line++;

        // Get the comment string that was read at the specified line. 
        char *comment = getComment(image, *line);

        // Write the comment to the file if not NULL. Comments are already terminated with \n.
        if (comment != NULL)
            fputs(comment, file);

        count++;
        // Loop while there exists a comment on the next line.
    } while (getCommentExists(image, *line + 1) == 1);
}