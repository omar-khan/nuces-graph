#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct nGraph B = newGraph("B");
	listAllTrees(&B, 5);

	return 0;
}
