#include "nucesGraph.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	struct nGraph S1 = newStar("S", 5);						// Star Graph
	struct nGraph K1 = newCompleteGraph("B", 5);  // Complete Graph
	struct nGraph P1 = newPath("P", 5);           // Path Graph
	struct nGraph T1 = newRandomTree("T", 10);    // Random Tree
	struct nGraph R1 = newErdosRenyiGNP("G", 50, .01); // G(N,P)
	struct nGraph R2 = newErdosRenyiGNM("G", 50, 20);  // G(N,M)

	show(&S1);
	show(&K1);
	show(&P1);
	show(&T1);
	show(&R1);
	show(&R2);

	return 0;
}
