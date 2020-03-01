#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph L1 = newGraph("L1");
	addVertex(&L1, 1);
	addVertex(&L1, 2);
	addVertex(&L1, 3);
	addVertex(&L1, 4);
	addEdge(&L1, 1, 2, 0);
	addEdge(&L1, 2, 3, 0);
	addEdge(&L1, 3, 4, 0);
	addEdge(&L1, 4, 1, 0);

	struct nGraph L2 = newGraph("L2");
	addVertex(&L2, 1);
	addVertex(&L2, 2);
	addEdge(&L2, 1, 2, 0);

	struct nGraph L3 = crossProduct(&L1, &L2);
	show(&L3);

	struct nGraph L4 = crossProduct(&L3, &L2);
	show(&L4);
	
	return 0;
}
