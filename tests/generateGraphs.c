#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph S1 = newStar("S", 5);						// Star Graph
	struct nGraph K1 = newCompleteGraph("B", 5);  // Complete Graph
	struct nGraph P1 = newPath("P", 5);           // Path Graph

	show(&S1);
	show(&K1);
	show(&P1);

	return 0;
}
