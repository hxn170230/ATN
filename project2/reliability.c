#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "combinations.h"
#include "graphutils.h"

/* Function to calculate probability of each link */
void setReliabilityOfEdge(double *reliabilities, int numEdges, double p_base) {
	int i = 0;
	double magic[] = {2,0,2,1,3,4,6,8,3,5};
	// get probability as p ^ ceil(magic[i] / 3)
	for (i = 0; i < numEdges; i++) {
		reliabilities[i] = pow(p_base,  (ceil(magic[i]/3)));
	}
}

/* Returns reliability of the link based on up/down state */
double getProbability(char *c, int num, double *reliabilities) {
	double prob = 1;
	int i = 0;
	while (i < num) {
		if (c[i] == '0') {
			prob = prob *reliabilities[i];
		} else {
			prob = prob *(1-reliabilities[i]);
		}
		i++;
	}
	return prob;
}

/* Returns 1 if graph of network is disconnected. Returns 0 otherwise */
int isSystemDown(int numNodes, char *c, int num, int edges[][2]) {
	int edgeMatrix[numNodes][numNodes];
	graph_utils_construct(numNodes, edgeMatrix, c, num, edges);
	if (graph_utils_disconnected(numNodes, edgeMatrix) == 1) {
		return 1;
	} else {
		return 0;
	}
}

/* Reliability calculator using all possible combinations of links' states */
double calculateReliability(int numNodes, int numEdges, double *reliabilities, int edges[][2]) {
	int i = 0;
	double reliability = 0;
	int upCount = 0;
	int downCount = 0;

	for (i = 0; i < numEdges; i++) {
		// all combinations are returned as linked list
		// get combinations of i down links
		// up --> 0
		// down --> 1
		llist_st *list = getCombinations(0, numEdges-1, i);
		llist_st *temp = list;
		while (temp != NULL) {
			// for each combination, check if graph of the network is disconnected
			if (isSystemDown(numNodes, temp->c, numEdges, edges) == 0) {
				// if graph is not disconnected, add reliability of combination to total reliability
				reliability = reliability + getProbability(temp->c, numEdges, reliabilities);
				upCount++;
			} else {
				downCount++;
			}
			temp = temp->next;
		}
		free_list(list);
	}
	printf("UP: %d DOWN: %d\n", upCount, downCount);
	return reliability;
}

llist_st *getSequence(llist_st *list, int seq) {
	while (seq > 0 && list != NULL) {
		list = list->next;
		seq -= 1;
	}
	return list;
}

char *switchStates(char *c, int numEdges) {
	int i = 0;
	char *newC = (char *)malloc(sizeof(char)*(strlen(c)+1));
	newC[strlen(c)] = '\0';
	while (i < numEdges) {
		if (c[i] == '0') {
			newC[i] = '1';
		} else {
			newC[i] = '0';
		}
		i++;
	}
	return newC;
}

int main() {
	// get number of nodes
	int numNodes = 0;
	scanf("%d", &numNodes);

	// get number of edges
	int numEdges = 0;
	scanf("%d", &numEdges);

	// is graph undirected
	int isUndirected = 0;
	scanf("%d", &isUndirected);
	
	int i = 0;
	// Edge list
	int edges[numEdges][2];
	for (i = 0; i < numEdges; i++) {
		int s = 0;
		int d = 0;
		scanf("%d %d", &s, &d);
		edges[i][0] = s-1;
		edges[i][1] = d-1;
	}

	double p_base = 0;
	scanf("%lf", &p_base);
	double p = p_base;
	printf("Probability \t Reliability\n");
	// case 1: probability of each link run from 0.05 to 1
	while (p <= 1.01) {
		// reliability of each link
		double reliabilities[numEdges];
		setReliabilityOfEdge(reliabilities, numEdges, p);
		// get the reliability of the system
		double reliability  = calculateReliability(numNodes, numEdges, reliabilities, edges);
		printf("%lf \t %lf\n", p, reliability);
		p = p + 0.05;
	}

	// case 2: fix p to 0.9, run 5 times for each k in [0, 20]
	// get all possible combinations of given nodes and edges
	// pick any random k combinations and flip the states
	// calculate reliability 
	p = 0.9;
	llist_st *list = NULL;
	int combinations = 0;
	double reliabilities[numEdges];
	double reliability = 0;
	setReliabilityOfEdge(reliabilities, numEdges, p);
	for (i = 0; i <= numEdges; i++) {
		llist_st *temp = getCombinations(0, numEdges-1, i);
		llist_st *temp1 = temp;
		while (temp1->next != NULL) {
			temp1 = temp1->next;
			combinations ++;
		}
		combinations++;
		temp1->next = list;
		list = temp;
	}
	list = reverseList(list);
	llist_st *temp = list;
	while (temp != NULL) {
		if (isSystemDown(numNodes, temp->c, numEdges, edges) == 0) {
			reliability = reliability + getProbability(temp->c, numEdges, reliabilities);
		}
		temp = temp->next;
	}
	printf("Total combinations: %d Reliability: %lf\n", combinations, reliability);
	int k = 0;
	for (k = 0; k <= 20; k++) {
		int j = 0;
		double total_reliability = 0;
		for (i = 0; i < 1000; i++) {
			double local_reliability = reliability;
			for (j = 0; j < k; j++) {
				int seq = rand()%combinations;
				llist_st *comb = getSequence(list, seq);
				char *comb_c = comb->c;
				double prob = getProbability(comb_c, numEdges, reliabilities);
				if (isSystemDown(numNodes, comb_c, numEdges, edges) == 1) {
					local_reliability += prob;
				} else {
					local_reliability -= prob;
				}
				local_reliability = local_reliability<0?0:local_reliability;
			}
			total_reliability = total_reliability + local_reliability;
			total_reliability = total_reliability<0?0:total_reliability;
		}
		//printf("k: %d Reliability: %lf\n", k, total_reliability/1000);
		printf("%lf\n", total_reliability/1000);
	}
	free_list(list);
}
