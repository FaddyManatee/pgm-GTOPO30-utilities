all: pgmEcho # pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble

pgmEcho: pgmEcho.o pgmio.o pgmerror.o pgmdata.o
    gcc pgmEcho.o pgmio.o pgmerror.o pgmdata.o -o pgmEcho -g -Wall -Werror

pgmEcho.o: pgmEcho.c
    gcc pgmEcho.c -c -Wall -Werror

pgmio.o: pgmio.c pgmdata.h pgmerror.h pgmlimits.h
    gcc pgmio.c -c -Wall -Werror

pgmerror.o: pgmerror.c pgmdata.h pgmexit.h pgmlimits.h
    gcc pgmerror.c -c -Wall -Werror

pgmdata.o: pgmdata.c pgmlimits.h
    gcc pgmdata.c -c -Wall -Werror

clean:
    rm *.o pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble
    