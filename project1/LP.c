#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lp.h"

void print(int nodes, int a[][nodes]) {
	int i = 0;
	int j = 0;
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

void initializeA(int nodes, int a[][nodes], int k) {
	int i = 0;
	int j = 0;
	int l = 0;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			a[i][j] = INT_MAX;
		}
	}

	// for all i
	// 	get k random l
	// 		set a[i][l] = 1
	// 	if j is not in l, a[i][j] = 300
	for (i = 0; i < nodes; i++) {
		a[i][i] = 0;
		for (l = 0; l < k; l++) {
			int j_i = rand()%nodes;
			while (j_i == i) {
				j_i = rand()%nodes;
			}
			a[i][j_i] = 1;
		}
		for (j = 0; j < nodes; j++) {
			if (a[i][j] != 1 && i != j) {
				a[i][j] = 300;
			}
		}
	}
}

void initializeB(int nodes, int b[][nodes], char *id) {
	int i = 0;
	int j = 0;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			b[i][j] = 0;
			b[i][j] = abs((id[i]-'0') - (id[j]-'0'));
		}
	}
}

void get_capacities(int nodes, int z[][nodes], 
		int a[][nodes], int b[][nodes]) {
	int i = 0;
	int j = 0;
	int l = 0;
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (i == j) {
				z[i][j] = 0;
			} else {
				z[i][j] = b[i][j]*a[i][j];
			}
		}
	}

	int m = 0;
	while (m < nodes) {
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (i != j) {
				int temp = INT_MAX;
				for (l = 0; l < nodes; l++) {
					temp = b[i][j]*(a[i][l] + a[l][j]);
					if (temp < z[i][j]) {
						z[i][j] = temp;
					}
				}
			}
		}
	}
	m++;
	}
}

int get_opt_z(int nodes, int z[][nodes]) {
	int i = 0;
	int j = 0;
	int z_opt = 0;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			z_opt += z[i][j];
		}
	}

	return z_opt;
}

void form_topologies(int nodes, int a[][nodes]) {
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;

	while (l < nodes) {
		for (i = 0; i < nodes; i++) {
			if (i != j) {
				for (j = 0; j < nodes; j++) {
					int temp = INT_MAX;
					for (k = 0; k < nodes; k++) {
						temp = a[i][k] + a[k][j];
						if (temp < a[i][j]) {
							a[i][j] = temp;
						}	
					}
				}
			}
		}
		l++;
	}
}

float get_density(int nodes, int z[][nodes]) {
	int i = 0;
	int j = 0;
	float n = 0;
	float total = (float)(nodes * (nodes-1));
	float density;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (z[i][j] != 0) {
				n++;
			}
		}
	}

	//printf("Number of non zero capacities: %f\n", n);
	density = n/total;
	return density;
}

void printGraph(int nodes, int z[][nodes]) {
	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			printf("%d	%d	Directed	%d	%d\n", i, j, k++, z[i][j]);
		}
	}
}

int main() {
	int nodes = 0;
	int k = K_START;
	int k_max = K_END;
	char *id = "202134683520213468352021346835";
	int index = k;

	scanf("%d", &nodes);
	for (index = k; index < k_max; index++) {
		int a[nodes][nodes];
		int b[nodes][nodes];

		initializeA(nodes, a, index);
		//printf("\nObtained Graph:\n");
		//print(nodes, a);

		initializeB(nodes, b, id);
		//printf("\nObtained B:\n");
		//print(nodes, b);
		form_topologies(nodes, a);
		printf("\nShortest Path Graph:\n");
		print(nodes, a);

		int z[nodes][nodes];
		get_capacities(nodes, z, a, b);
		printf("\nObtained Capacity Matrix:\n");
		print(nodes, z);

		int z_opt = get_opt_z(nodes, z);
		float density = get_density(nodes, z);
		printf("\nFor k(%d), z_opt(%d) density(%f)\n", index, z_opt, density);
	}
	return 1;
}
