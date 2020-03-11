#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	nGraph G = newGraph("G");

	int i;
	for (i = 0; i < 5; i++) 
		addVertex(&G, i);
	
	for (i = 0; i < 5; i++) 
		addRandomEdge(&G, 1);

	show(&G);
	showDot(&G);

	degreeHistogram(&G);
	adjacencyMatrix(&G);
	incidenceMatrix(&G);

	exportDimac(&G);

	return 0;
}

