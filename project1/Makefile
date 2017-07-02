lp: lp.o
	gcc -o lp lp.o
lp.o: LP.c lp.h
	gcc -c -Wall -DDEBUG LP.c -I.

clean:
	rm -rf lp.o lp
