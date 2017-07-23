#include "graph_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print(Edge_st *list) {
	while (list != NULL) {
		printf("(%d, %d, %lf) --> ", list->source, list->dest, list->val);
		list = list->next;
	}
	printf("NULL\n");
}

void printGraph(int n, Edge_st *AdjList[]) {
	int i = 0;
	for (i = 0; i < n; i++) {
		printf("Vertex(%d): ", i);
		print(AdjList[i]);
		printf("\n");
	}
}

int getLength(Edge_st *list) {
	int ret = 0;
	while (list != NULL) {
		list = list->next;
		ret += 1;
	}
	return ret;
}

Edge_st *merge(Edge_st *first, Edge_st *second) {
	Edge_st *merged = NULL;
	Edge_st *temp = NULL;
	
	while (first && second) {
		if (first->val < second->val) {
			if (merged == NULL) {
				merged = first;
				temp = merged;
			} else {
				temp->next = first;
				temp = temp->next;
			}
			first = first->next;
		} else {
			if (merged == NULL) {
				merged = second;
				temp = merged;	
			} else {
				temp->next = second;
				temp = temp->next;
			}
			second = second->next;
		}
	}

	while (first) {
		temp->next = first;
		temp = temp->next;
		first = first->next;
	}

	while (second) {
		temp->next = second;
		temp = temp->next;
		second = second->next;
	}
	return merged;
}

Edge_st *mergeSort(Edge_st *list, int start, int end) {
	if (end-start == 0) {
		return list;
	}
	if (end-start == 1) {
		Edge_st *startL = list;
		Edge_st *endL = list->next;
		startL->next = NULL;
		Edge_st *llist = merge(startL, endL);
		return llist;
	} else {
		int mid = start + (end-start)/2;
		int i = start;
		Edge_st *temp = list;
		while (i < mid) {
			temp = temp->next;
			i+=1;	
		}
		Edge_st *endL = temp->next;
		temp->next = NULL;
		Edge_st *startL = mergeSort(list, start, mid);
		Edge_st *midL = mergeSort(endL, mid+1, end);	
		Edge_st *ret = merge(startL, midL);
		return ret;
	}
}

Edge_st *sortList(Edge_st *list) {
	int n = getLength(list);
	return mergeSort(list, 0, n-1);
}

Edge_st *getSortedEdgeList(int n, Edge_st *FullAdjList[n]) {
	int i = 0;
	Edge_st *list = NULL;
	for (i = 0; i < n; i++) {
		if (list == NULL) {
			list = FullAdjList[i];
		} else {
			Edge_st *temp = list;
			while (temp->next != NULL) {
				temp = temp->next;
			}
			Edge_st *l = FullAdjList[i];
			temp->next = l;
		}
	}
	print(list);
	list = sortList(list);
	return list;
}

void insert(queue_st *q, int vertex) {
	if (q->back == (q->front+1)%1000) {
		printf("Queue full\n");
		return;
	} else {
		q->list[q->front] = vertex;
		q->front = q->front + 1;
	}
}

 int remove_q(queue_st *q) {
	if (q->back == q->front) {
		return -1;
	} else {
		int edge = q->list[q->back];
		q->back = q->back+1;
		return edge;
	}
}

int isGraphConnected(int n, Edge_st *AdjList[n]) {
	int visited[n];
	int i = 0;
	if (AdjList == NULL) {
		return -1;
	}
	for (i = 0; i < n; i++) {
		visited[i] = 0;
	}
	queue_st q;
	q.front = 0;
	q.back = 0;

	i = 0;
	while (AdjList[i] == NULL && i < n) {
		i++;
	}
	if (i >= n) {
		return -1;
	}

	insert(&q, i);
	int val = remove_q(&q);
	while (val >= 0) {
		visited[val] = 1;
		Edge_st *edges = AdjList[val];
		while (edges) {
			if (visited[edges->dest] != 1)
				insert(&q, edges->dest);
			edges = edges->next;
		}
		val = remove_q(&q);
	}

	for (i = 0; i < n; i++) {
		if (visited[i] != 1) {
			printf("Graph not connected at %d\n", i);
			return i;
		}
	}
	printf("Graph is connected\n");
	return -2;
}

void printHops(int n, int hops[][n]) {
	int i = 0;
	int j = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", hops[i][j]);
		}
		printf("\n");
	}
}

void initialize(int n, Edge_st *AdjList[n], int hops[][n]) {
	int i = 0;
	int j = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			hops[i][j] = INT_MAX;
		}
		hops[i][i] = 0;
	}

	for (i = 0; i < n; i++) {
		Edge_st *edges = AdjList[i];
		while (edges) {
			j = edges->dest;
			hops[i][j] = 1;
			edges = edges->next;
		}
	}
}

int getDiameter(int n, Edge_st *AdjList[n], int *s, int *d, int hops[][n]) {
	// construct the hop matrix
	int i = 0;
	int j = 0;
	int maxDist = 0;

	// Run floyd warshall algorithm
	int k = 0;
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (hops[i][k] < INT_MAX && hops[k][j] < INT_MAX) {
					if (hops[i][k] + hops[k][j] < hops[i][j]) {
						hops[i][j] = hops[i][k] + hops[k][j];
					}
				}
			}
		}
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (maxDist < hops[i][j]) {
				maxDist = hops[i][j];
				*s = i;
				*d = j;
			}
		}
	}
	//printHops(n, hops);
	return maxDist;
}
