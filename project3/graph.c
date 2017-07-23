#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph_utils.h"
#include "graph.h"

double getDistance(int vi, int vj, int n, double nxy[][2]) {
	double x1 = nxy[vi][0];
	double y1 = nxy[vi][1];

	double x2 = nxy[vj][0];
	double y2 = nxy[vj][1];

	return (sqrt(((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2))));	
}

double costOfGraph(int n, Edge_st *AdjList[n]) {
	double cost = 0;
	int i = 0;
	for (i = 0; i < n; i++) {
		Edge_st *list = AdjList[i];
		while (list) {
			cost += list->val;
			list = list->next;
		}
	}
	return cost;
}

void getRandomSamples(int n, double nxy[][2]) {
	int i = 0;
	for (i = 0; i < n; i++) {
		double samplex = rand()%10000;
		double sampley = rand()%10000;
		nxy[i][0] = samplex;
		nxy[i][1] = sampley;
	}
}

Edge_st *findMinEdge(int n, int source, int dest, vertices_st *hops[], double nxy[][2]) {
	Edge_st *edge = (Edge_st *)malloc(sizeof(Edge_st));
	edge->source = source;
	edge->dest = dest;
	edge->val = getDistance(source, dest, n, nxy);
	edge->next = NULL;
	int i = 0;
	for (i = 0; i < MAXDIAG-1; i++) {
		vertices_st *vlist = hops[i];
		while (vlist) {
			int s = vlist->id;
			int d = dest;
			double val = getDistance(s, d, n, nxy);
			if (val < edge->val) {
				edge->source = s;
				edge->dest = d;
				edge->val = val;
			}
			vlist = vlist->next;
		}
	}
	return edge;
}

void algo1(int n, Edge_st *FullAdjList[], double nxy[][2], Edge_st *AdjList[n]) {
	int i = 0;
	int degree[n];
	for (i = 0; i < n; i++) {
		degree[i] = 0;
		AdjList[i] = NULL;
	}
	Edge_st * list = getSortedEdgeList(n, FullAdjList);
	Edge_st *l = list;
	Edge_st *l_parent = NULL;
	while (isGraphConnected(n, AdjList) != -2 && l) {
		Edge_st *edge = l;
		int vi = edge->source;
		int vj = edge->dest;
		if (degree[vi] < 3 || degree[vj] < 3) {
			if (l_parent == NULL) {
				list = list->next;
				l = list;
			} else {
				l_parent->next = l->next;
				l = l->next;
			}

			edge->next = AdjList[vi];
			AdjList[vi] = edge;
			degree[vi] += 1;

			Edge_st *second = (Edge_st *)malloc(sizeof(Edge_st));
			second->source = vj;
			second->dest = vi;
			second->val = edge->val;
			second->next = AdjList[vj];
			AdjList[vj] = second;
			degree[vj] += 1;

		} else {
			l_parent = l;
			l = l->next;
		}
	}

	printf("STAGE 1: graph\n");
	printGraph(n, AdjList);

	int index = isGraphConnected(n, AdjList);
	while (index >= 0) {
		Edge_st *edge = list;
		Edge_st *l_parent = NULL;
		while (edge) {
			if (edge->source == index) {
				break;
			}
			l_parent = edge;
			edge = edge->next;
		}
		if (edge != NULL) {
			if (l_parent == NULL) {
				list = list->next;
			} else {
				l_parent->next = edge->next;
			}
			edge->next = AdjList[edge->source];
			AdjList[edge->source] = edge;
			degree[edge->source] += 1;

			Edge_st *second = (Edge_st *)malloc(sizeof(Edge_st));
			second->source = edge->dest;
			second->dest = edge->source;
			second->val = edge->val;
			second->next = AdjList[second->source];
			AdjList[second->source] = second;
			degree[second->source] += 1;
		}
		index = isGraphConnected(n, AdjList);
	}

	// check degree of each vertex
	for (i = 0; i < n; i++) {
		while (degree[i] < MAXDEGREE && list != NULL) {
			// degree is less than 3
			Edge_st *temp = list;
			Edge_st *temp_prev = NULL;
			while (temp) {
				if (temp->source == i || temp->dest == i) {
					if (temp_prev != NULL) {
						temp_prev->next = temp->next;
					} else {
						list = temp->next;
					}
					temp->next = NULL;
					break;
				} else {
					temp_prev = temp;
					temp = temp->next;
				}
			}
			Edge_st *vilist = AdjList[temp->source];
			Edge_st *t = vilist;
			while (t->next) {
				t = t->next;
			}
			t->next = temp;
			AdjList[temp->source] = vilist;
			degree[temp->source] += 1;

			Edge_st *second = (Edge_st *)malloc(sizeof(Edge_st));
			second->source = temp->dest;
			second->dest = temp->source;
			second->val = temp->val;
			Edge_st *vjlist = AdjList[temp->dest];
			t = vjlist;
			while (t->next) {
				t = t->next;
			}
			t->next = second;
			AdjList[temp->dest] = vjlist;
			degree[temp->dest] += 1;
		}
	}

	printf("STAGE 2: \n");
	printGraph(n, AdjList);

	int sDiag = -1;
	int dDiag = -1;
	int hops[n][n];
	initialize(n, AdjList, hops);
	int diameter = getDiameter(n, AdjList, &sDiag, &dDiag, hops);
	while (diameter > MAXDIAG) {
		printf("Diameter: %d source: %d destination: %d\n", diameter, sDiag, dDiag);
		// get 1,2,3,...,MAXDIAG-1 hop destinations from source
		vertices_st *hop[MAXDIAG-1];
		for (i = 0; i < MAXDIAG-1; i++) {
			hop[i] = NULL;
		}
		for (i = 0; i < n; i++) {
			if (hops[sDiag][i] < MAXDIAG && i != sDiag) {
				vertices_st *vlist = hop[hops[sDiag][i]-1];
				vertices_st *v = (vertices_st *)malloc(sizeof(vertices_st));
				v->id = i;
				v->next = vlist;
				hop[hops[sDiag][i]-1] = v;
			}
		}
		// get min edge from hop destinations to destination
		Edge_st *edge = findMinEdge(n, sDiag, dDiag, hop, nxy);
		// add min edge 
		Edge_st *vitemp = AdjList[edge->source];
		Edge_st *t = vitemp;
		while (t->next) {
			t = t->next;
		}
		t->next = edge;
		Edge_st *second = (Edge_st *)malloc(sizeof(Edge_st));
		second->source = edge->dest;
		second->dest = edge->source;
		second->val = edge->val;
		second->next = NULL;
		Edge_st *vjtemp = AdjList[edge->dest];
		t = vjtemp;
		while (t->next) {
			t = t->next;
		}
		t->next = second;
		AdjList[edge->dest] = vjtemp;
		degree[edge->source] += 1;
		degree[edge->dest] += 1;
		hops[edge->source][edge->dest] = 1;
		hops[edge->dest][edge->source] = 1;
		diameter = getDiameter(n, AdjList, &sDiag, &dDiag, hops);
	}
	printHops(n, hops);
}

int main() {
	int n = 0;
	scanf("%d", &n);

	double nxy[n][2];
	int i = 0;
	/*for (i = 0; i < n; i++) {
		scanf("%lf %lf", &nxy[i][0], &nxy[i][1]);
	}*/
	getRandomSamples(n, nxy);

	Edge_st *FullAdjList[n];
	for (i = 0; i < n; i++) {
		FullAdjList[i] = NULL;
		Edge_st *edgeI = NULL;
		int j = 0;
		for (j = i+1; j < n; j++) {
			edgeI = (Edge_st *)malloc(sizeof(Edge_st));	
			edgeI->source = i;
			edgeI->dest = j;
			edgeI->val = getDistance(i, j, n, nxy);
			edgeI->next = FullAdjList[i];
			FullAdjList[i] = edgeI;
		}
	}

	Edge_st *AdjList[n];
	algo1(n, FullAdjList, nxy, AdjList);
	printf("Graph Obtained with CONSTRAINTS: \n ");
	printGraph(n, AdjList);
	printf("Cost of the graph: %lf\n", costOfGraph(n, AdjList));
	return 1;
}
