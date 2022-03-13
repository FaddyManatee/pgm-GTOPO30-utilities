all: pgmEcho # pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble

pgmEcho: pgmEcho.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmEcho.o pgmio.o pgmerror.o pgmdata.o -o pgmEcho -g

pgmEcho.o: pgmEcho.c
	gcc pgmEcho.c -c -g

pgmio.o: pgmio.c pgmdata.h pgmerror.h pgmlimits.h
	gcc pgmio.c -c -g

pgmerror.o: pgmerror.c pgmdata.h pgmexit.h pgmlimits.h
	gcc pgmerror.c -c -g

pgmdata.o: pgmdata.c pgmlimits.h
	gcc pgmdata.c -c -g

clean:
	rm *.o pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble
		