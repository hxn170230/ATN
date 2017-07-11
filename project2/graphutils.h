#ifndef _GRAPH_UTILS_H
#define _GRAPH_UTILS_H

/* Construct graph using edges and character array depicting links up/down */
void graph_utils_construct(int numNodes, int edgeMatrix[][numNodes], char *c, int numEdges, int edges[][2]);
/* Check if the graph given by edge matrix is disconnected i.e. is any node not reachable */
int graph_utils_disconnected(int numNodes, int edgeMatrix[][numNodes]);

typedef struct queue {
	int list[100];
	int front;
	int back;
}queue_st;

#endif
