#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("G");

	addVertex(&G, 1);
	addVertex(&G, 2);
	addVertex(&G, 3);
	addVertex(&G, 4);
	addVertex(&G, 5);
	addVertex(&G, 6);

	addEdge(&G, 1, 2, 0);
	addEdge(&G, 5, 1, 0);
	addEdge(&G, 2, 5, 0);
	addEdge(&G, 2, 3, 0);
	addEdge(&G, 4, 5, 0);
	addEdge(&G, 3, 4, 0);
	addEdge(&G, 4, 6, 0);

	struct nGraph P = getVertices(&G);
	struct nGraph R = newGraph("R");
	struct nGraph X = newGraph("X");

	show(&G);

	BronKerbosch(&G, &P, &R, &X);

	return 0;
}
