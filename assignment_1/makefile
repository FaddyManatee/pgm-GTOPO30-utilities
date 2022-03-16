all: pgmEcho pgma2b pgmb2a pgmComp pgmReduce pgmTile #pgmAssemble

pgmEcho: pgmEcho.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmEcho.o pgmio.o pgmerror.o pgmdata.o -o pgmEcho -g

pgmComp: pgmComp.o pgmcompare.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmComp.o pgmcompare.o pgmio.o pgmerror.o pgmdata.o -o pgmComp -g

pgma2b: pgma2b.o pgmio.o pgmerror.o pgmdata.o
	gcc pgma2b.o pgmio.o pgmerror.o pgmdata.o -o pgma2b -g

pgmb2a: pgmb2a.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmb2a.o pgmio.o pgmerror.o pgmdata.o -o pgmb2a -g

pgmReduce: pgmReduce.o pgmshrink.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmReduce.o pgmshrink.o pgmio.o pgmerror.o pgmdata.o -o pgmReduce -g

pgmTile: pgmTile.o pgmgroup.o pgmio.o pgmerror.o pgmdata.o
	gcc pgmTile.o pgmgroup.o pgmio.o pgmerror.o pgmdata.o -o pgmTile -g

pgmEcho.o: pgmEcho.c
	gcc pgmEcho.c -c -g

pgmComp.o: pgmComp.c
	gcc pgmComp.c -c -g

pgmReduce.o: pgmReduce.c
	gcc pgmReduce.c -c -g

pgmTile.o: pgmTile.c
	gcc pgmTile.c -c -g

pgma2b.o: pgma2b.c
	gcc pgma2b.c -c -g

pgmb2a.o: pgmb2a.c
	gcc pgmb2a.c -c -g

pgmio.o: pgmio.c pgmdata.h pgmerror.h pgmlimits.h
	gcc pgmio.c -c -g

pgmerror.o: pgmerror.c pgmdata.h pgmexit.h pgmlimits.h
	gcc pgmerror.c -c -g

pgmdata.o: pgmdata.c pgmlimits.h
	gcc pgmdata.c -c -g

pgmcompare.o: pgmcompare.c pgmdata.h
	gcc pgmcompare.c -c -g

pgmshrink.o: pgmshrink.c pgmdata.h
	gcc pgmshrink.c -c -g

pgmgroup.o: pgmgroup.c pgmdata.h
	gcc pgmgroup.c -c -g

clean:
	rm *.o pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble
		