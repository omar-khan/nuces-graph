#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("G");

	int i;
	for (i = 0; i < 20; i++) 
		addVertex(&G, i);
	
	addVertexLabel(&G, "hello");

	for (i = 0; i < 800; i++) 
		addRandomEdge(&G, 0);

	show(&G);
	showDot(&G);

	degreeHistogram(&G);

	return 0;
}

