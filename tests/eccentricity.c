#include <stdio.h>
#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("G");

	int i;
	for (i = 0; i < 9; i++) {
		addVertex(&G, i);
	}

	addEdge(&G, 0, 1, 4);
	addEdge(&G, 0, 7, 8);
	addEdge(&G, 1, 7, 11);
	addEdge(&G, 1, 2, 8);
	addEdge(&G, 7, 8, 7);
	addEdge(&G, 7, 6, 1);
	addEdge(&G, 2, 8, 2);
	addEdge(&G, 8, 6, 6);
	addEdge(&G, 2, 3, 7);
	addEdge(&G, 2, 5, 4);
	addEdge(&G, 6, 5, 2);
	addEdge(&G, 3, 5, 14);
	addEdge(&G, 3, 4, 9);
	addEdge(&G, 4, 5, 10);

	for (i = 0; i < 9; i++) {
		printf("E(%d) = %d\n", i, eccentricity(&G, i));
	}

	return 0;
}

