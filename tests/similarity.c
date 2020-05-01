#include "nucesGraph.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
#if 0
	struct nGraph home = newCompleteGraph("H",4);
	addVertex(&home, 4);
	addEdge(&home, 2, 4, 1);
	addEdge(&home, 3, 4, 1);
#endif
	struct nGraph home = newWheel("W", 17);

	int  i, j;

	/* Common Neighbors */
#if 0
	int commonNeigh[home.V->count][home.V->count];
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			commonNeigh[i][j] = commonNeighbors(&home, i, j);
			//printf("%d Common Neighbors of %d and %d\n", commonNeigh[i][j], i, j);
		}
	}
#endif

	/* Jaccard Similarity */	
	double jaccard[home.V->count*home.V->count];
	for (j = 0; j < home.V->count; j++) {
		for (i = 0; i < home.V->count; i++) {
			jaccard[j*home.V->count+i] = jaccardCoefficient(&home, i, j);
			printf("%4.4f\t", jaccard[j*home.V->count+i]);
		}
		printf("\n");
	}

	/* Adamic-Adar Index */
#if 0
	double adamicAdar[home.V->count][home.V->count];
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			adamicAdar[i][j] = adamicAdarIndex(&home, i, j);
			//printf("%f Adamic-Adar Index of %d and %d\n", adamicAdar[i][j], i, j);
		}
	}

	/* Common Neighbors as Vertices */
	struct nGraph common = getCommonNeighbors(&home, 2, 3);
	show(&common);
#endif

	/* Learning Example */
	showDot(&home);
	int before = home.E->count;

	adjacencyMatrix(&home);

	double sum = 0;
	for (j = 0; j < home.V->count; j++) {
		for (i = 0; i < home.V->count; i++) {
			sum += pow(abs(jaccard[j*home.V->count+i] - home.adjacency_matrix[j*home.V->count+i]),2);
		}
	}
	printf("Loss Function: %f\n", sum);

	removeRandomEdges(&home, 1);

	int after  = home.E->count;
	printf("before: %d\nafter: %d\n", before, after);

	double jaccard2[home.V->count*home.V->count];
	for (j = 0; j < home.V->count; j++) {
		for (i = 0; i < home.V->count; i++) {
			jaccard2[j*home.V->count+i] = jaccardCoefficient(&home, i, j);
			printf("%4.4f\t", jaccard2[j*home.V->count+i]);
		}
		printf("\n");
	}
	showDot(&home);

	adjacencyMatrix(&home);

	sum = 0;
	for (j = 0; j < home.V->count; j++) {
		for (i = 0; i < home.V->count; i++) {
			sum += pow(abs(jaccard2[j*home.V->count+i] - home.adjacency_matrix[j*home.V->count+i]),2);
		}
	}
	printf("Loss Function: %f\n", sum);

	return 0;
}
