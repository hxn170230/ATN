#include <stdio.h>
#include "graphutils.h"

void initialize(int numNodes, int edgeMatrix[][numNodes]) {
	int i = 0;
	int j = 0;
	for (i = 0; i < numNodes; i++) {
		for (j = 0; j < numNodes; j++) {
			edgeMatrix[i][j] = 0;
		}
	}
}

/* Construct graph using edges and character array depicting links up/down */
void graph_utils_construct(int numNodes, int edgeMatrix[][numNodes], char *c, int numEdges, int edges[][2]) {
	initialize(numNodes, edgeMatrix);
	int i = 0;
	for (i = 0; i < numEdges; i++) {
		int s = edges[i][0];
		int d = edges[i][1];
		if (c[i] == '0') {
			// link is not down
			// assuming undirected graph
			edgeMatrix[s][d] = 1;
			edgeMatrix[d][s] = 1;
		}
	}
}

void queue_insert(queue_st *q, int val) {
	if (q->back == (q->front+1)%100) {
	} else {
		q->list[q->front] = val;
		q->front += 1;
	}
}

int queue_remove(queue_st *q) {
	if (q->front == q->back) {
		return -1;
	} else {
		int val = q->list[q->back];
		q->back += 1;
		return val;
	}
}

/* Check if the graph given by edge matrix is disconnected i.e. is any node not reachable */
int graph_utils_disconnected(int numNodes, int edgeMatrix[][numNodes]) {
	// assuming undirected graph
	int i = 0;
	// run BFS
	int vertex[numNodes];
	for (i = 0; i < numNodes; i++) {
		vertex[i] = 0;
	}
	queue_st q;
	q.front = 0;
	q.back = 0;
	queue_insert(&q, 0);
	vertex[0] = 1;
	int val = queue_remove(&q);
	while (val != -1) {
		int j = 0;
		for (j = 0; j < numNodes; j++) {
			if (edgeMatrix[val][j] != 0 && vertex[j] != 1) {
				queue_insert(&q, j);
				vertex[j] = 1;
			}
		}
		val = queue_remove(&q);
	}

	for (i = 0; i < numNodes; i++) {
		if (vertex[i] == 0) {
			return 1;
		}
	}
	return 0;
}
