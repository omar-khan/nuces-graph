#include <stdio.h>
#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("G");

	int i;
	for (i = 0; i < 9; i++) {
		addVertex(&G, i);
	}

	addEdge(&G, 0, 1, 1);
	addEdge(&G, 0, 7, 1);
	addEdge(&G, 1, 7, 1);
	addEdge(&G, 1, 2, 1);
	addEdge(&G, 7, 8, 1);
	addEdge(&G, 7, 6, 1);
	addEdge(&G, 2, 8, 1);
	addEdge(&G, 8, 6, 1);
	addEdge(&G, 2, 3, 1);
	addEdge(&G, 2, 5, 1);
	addEdge(&G, 6, 5, 1);
	addEdge(&G, 3, 5, 1);
	addEdge(&G, 3, 1, 1);
	addEdge(&G, 1, 5, 1);
	addEdge(&G, 0, 4, 1);
	
	//showDot(&G);

	int j;
	for (j = 0; j < 9; j++) {
		for (i = j; i < 9; i++) {
			if (i != j) {
				printf("length: %d -> %d = %d\n", i, j, shortestPathLength(&G, i, j));
			}
		}
	}

	return 0;
}

