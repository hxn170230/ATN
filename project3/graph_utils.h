#ifndef __GRAPH_UTILS_H
#define __GRAPH_UTILS_H

typedef struct edge{
	int source;
	int dest;
	double val;
	struct edge *next;
}Edge_st;

typedef struct {
	int front;
	int back;
	int list[1000];
}queue_st;

typedef struct vertex {
	int id;
	struct vertex *next;
}vertices_st;

// make sure to remove duplicate undirected edges
extern Edge_st *getSortedEdgeList(int n, Edge_st *FullAdjList[n]);
extern int isGraphConnected(int n, Edge_st *AdjList[n]);
extern void print(Edge_st *list);
extern void printGraph(int n, Edge_st *AdjList[]);
extern void printHops(int n, int hops[][n]);
extern int getDiameter(int n, Edge_st *AdjList[], int *source, int *destination, int hops[][n]);
extern void initialize(int n, Edge_st *AdjList[n], int hops[][n]);

#endif
