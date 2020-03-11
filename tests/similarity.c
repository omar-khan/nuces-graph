#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct nGraph home = newCompleteGraph("H",4);
	addVertex(&home, 4);
	addEdge(&home, 2, 4, 1);
	addEdge(&home, 3, 4, 1);

	int  i, j;

	/* Common Neighbors */	
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			printf("%d Common Neighbors of %d and %d\n", commonNeighbors(&home, i, j), i, j);
		}
	}

	/* Jaccard Similarity */	
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			printf("%f Jaccard Coefficient of %d and %d\n", jaccardCoefficient(&home, i, j), i, j);
		}
	}

	/* Adamic-Adar Index */
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			printf("%f Adamic-Adar Index of %d and %d\n", adamicAdarIndex(&home, i, j), i, j);
		}
	}

	/* Common Neighbors as Vertices */
	struct nGraph common = getCommonNeighbors(&home, 2, 3);
	show(&common);

	return 0;
}
