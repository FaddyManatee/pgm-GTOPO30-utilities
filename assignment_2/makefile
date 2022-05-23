all: gtopoEcho gtopoComp gtopoReduce gtopoTile gtopoAssemble

gtopoEcho: gtopoEcho.o gtopoio.o gtopoerror.o gtopodata.o
	gcc gtopoEcho.o gtopoio.o gtopoerror.o gtopodata.o -o gtopoEcho -g

gtopoComp: gtopoComp.o gtopocompare.o gtopoio.o gtopoerror.o gtopodata.o
	gcc gtopoComp.o gtopocompare.o gtopoio.o gtopoerror.o gtopodata.o -o gtopoComp -g

gtopoReduce: gtopoReduce.o gtoposhrink.o gtopoio.o gtopoerror.o gtopodata.o
	gcc gtopoReduce.o gtoposhrink.o gtopoio.o gtopoerror.o gtopodata.o -o gtopoReduce -g -lm

gtopoTile: gtopoTile.o gtopogroup.o gtopoio.o gtopoerror.o gtopodata.o
	gcc gtopoTile.o gtopogroup.o gtopoio.o gtopoerror.o gtopodata.o -o gtopoTile -g

gtopoAssemble: gtopoAssemble.o gtopogroup.o gtopoio.o gtopoerror.o gtopodata.o
	gcc gtopoAssemble.o gtopogroup.o gtopoio.o gtopoerror.o gtopodata.o -o gtopoAssemble -g

gtopoEcho.o: gtopoEcho.c
	gcc gtopoEcho.c -c -g

gtopoComp.o: gtopoComp.c
	gcc gtopoComp.c -c -g

gtopoReduce.o: gtopoReduce.c
	gcc gtopoReduce.c -c -g

gtopoTile.o: gtopoTile.c
	gcc gtopoTile.c -c -g

gtopoAssemble.o: gtopoAssemble.c
	gcc gtopoAssemble.c -c -g

gtopoio.o: gtopoio.c gtopodata.h gtopoerror.h gtopolimits.h
	gcc gtopoio.c -c -g

gtopoerror.o: gtopoerror.c gtopodata.h gtopoexit.h gtopolimits.h
	gcc gtopoerror.c -c -g

gtopodata.o: gtopodata.c gtopolimits.h
	gcc gtopodata.c -c -g

gtopocompare.o: gtopocompare.c gtopodata.h
	gcc gtopocompare.c -c -g

gtoposhrink.o: gtoposhrink.c gtopodata.h
	gcc gtoposhrink.c -c -g

gtopogroup.o: gtopogroup.c gtopodata.h
	gcc gtopogroup.c -c -g

clean:
	rm *.o gtopoEcho gtopoComp gtopoReduce gtopoTile gtopoAssemble
		