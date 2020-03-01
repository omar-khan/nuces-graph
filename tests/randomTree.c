#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct nGraph G = newGraph("T");
	makeRandomTree(&G, 100);

	struct nGraph B = newGraph("B");
	listAllTrees(&B, 5);

	return 0;
}
