#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("G");

	int i;
	for (i = 0; i < 10; i++) 
		addVertex(&G, i);
	
	addVertexLabel(&G, "hello");

	for (i = 0; i < 20; i++) 
		addRandomEdge(&G, 0);


	show(&G);
	exportDot(&G);
	showDot(&G);

	return 0;
}

