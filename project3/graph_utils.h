#ifndef __GRAPH_UTILS_H
#define __GRAPH_UTILS_H

/* Structure holding edge data */
typedef struct edge{
	int source;
	int dest;
	double val;
	struct edge *next;
}Edge_st;

/* Structure holding queue data */
typedef struct {
	int front;
	int back;
	int list[1000];
}queue_st;

/* Structure holding vertex data */
typedef struct vertex {
	int id;
	struct vertex *next;
}vertices_st;

// Function to return sorted edges list.
extern Edge_st *getSortedEdgeList(int n, Edge_st *FullAdjList[n]);
// Function to check graph connectivity
extern int isGraphConnected(int n, Edge_st *AdjList[n]);
// Function to print the list of edges
extern void print(Edge_st *list);
// Function to print the graph
extern void printGraph(int n, Edge_st *AdjList[]);
// Function to print the hops matrix
extern void printHops(int n, int hops[][n]);
// Function to calculate and return the diameter along with source and destination of diameter
extern int getDiameter(int n, Edge_st *AdjList[], int *source, int *destination, int hops[][n]);
// Function to initialize the hops matrix
extern void initialize(int n, Edge_st *AdjList[n], int hops[][n]);

#endif
