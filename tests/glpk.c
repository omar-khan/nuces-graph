#include "nucesGraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
	nGraph G = newGraph("G");

	int i;
	for (i = 1; i <= 7; i++) {
		addVertex(&G, i);
	}

	addEdgeDirected(&G, 0+1, 1+1, 32);
	addEdgeDirected(&G, 0+1, 2+1, 26);
	addEdgeDirected(&G, 1+1, 3+1, 24);
	addEdgeDirected(&G, 2+1, 4+1, 22);
	addEdgeDirected(&G, 2+1, 1+1, 8);
	addEdgeDirected(&G, 4+1, 3+1, 14);
	addEdgeDirected(&G, 3+1, 5+1, 40);
	addEdgeDirected(&G, 4+1, 5+1, 8);

	exportGLPK(&G);

	return 0;
}
