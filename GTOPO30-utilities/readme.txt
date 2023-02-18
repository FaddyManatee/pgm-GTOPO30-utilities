Running the submission

Running the makefile:
make <target>

Individual program targets: gtopoEcho, gtopoComp, gtopoReduce, gtopoTile, gtopoAssemble, gtopoPrintLand, gtopoAssembleReduce
All programs target: all
Delete .o and executables target: clean


Running the programs:
gtopoEcho: ./gtopoEcho inputFile width height outputFile
gtopoComp: ./gtopoComp firstFile width height secondFile
gtopoReduce: ./gtopoReduce input width height reduction_factor output
gtopoTile: ./gtopoTile inputFile width height tiling_factor outputFile_<row>_<column> -> (where <row> and <column> tags may appear anywhere in the output file name template)
gtopoAssemble: ./gtopoAssemble outputFile width height (row column inputFile width height)+
gtopoPrintLand: ./gtopoPrintLand inputFile width height outputFile sea hill mountain
gtopoAssembleReduce: ./gtopoAssembleReduce outputArray.gtopo width height reduction_factor (row column inputArray.gtopo width height)+ -> This takes approx. 2 minutes to compute entire GTOPO30 data

Running the test script
1: chmod +x testscript.sh
2: ./testscript.sh
