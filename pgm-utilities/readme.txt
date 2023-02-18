Running the submission

Running the makefile:
make <target>

Individual program targets: pgmEcho, pgmComp, pgma2b, pgmb2a, pgmReduce, pgmTile
All programs target: all
Delete .o and executables target: clean


Running the programs
pgmEcho: ./pgmEcho inputImage.pgm outputImage.pgm
pgmComp: ./pgmComp inputImage.pgm inputImage.pgm
pgma2b: ./pgma2b inputImage.pgm outputImage.pgm
pgmb2a: ./pgmb2a inputImage.pgm outputImage.pgm
pgmReduce: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm
pgmTile: ./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm (where <row> and <column> tags may appear anywhere in the output file name template)
pgmAssemble: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+

Running the test script
1: chmod +x testscript.sh
2: ./testscript.sh
