#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	struct nGraph S1 = newStar("S", 5);						// Star Graph
	struct nGraph K1 = newCompleteGraph("B", 5);  // Complete Graph
	struct nGraph P1 = newPath("P", 5);           // Path Graph
	struct nGraph T1 = newRandomTree("T", 100);   // Random Tree

	show(&S1);
	show(&K1);
	show(&P1);
	show(&T1);

	return 0;
}
