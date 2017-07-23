graph: graph.o graph_utils.o
	gcc -o graph graph.o graph_utils.o

graph.o: graph.c graph_utils.h graph.h
	gcc -c -Wall graph.c -I.

graph_utils.o: graph_utils.c graph_utils.h
	gcc -c -Wall graph_utils.c -I.

clean:
	rm -rf graph
	rm -rf graph.o graph_utils.o
