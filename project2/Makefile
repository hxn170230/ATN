reliability: reliability.o combinations.o graphutils.o
	gcc -o reliability reliability.o combinations.o graphutils.o

reliability.o: reliability.c combinations.h
	gcc -c -Wall reliability.c -I.

combinations.o:	combinations.c combinations.h
	gcc -c -Wall combinations.c -I.

graphutils.o: graphutils.c graphutils.h
	gcc -c -Wall graphutils.c -I.

clean:
	rm -rf combinations.o reliability.o graphutils.o
	rm -rf reliability
