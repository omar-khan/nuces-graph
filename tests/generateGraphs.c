#include "nucesGraph.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	struct nGraph S1 = newStar("S", 5);						// Star Graph
	struct nGraph K1 = newCompleteGraph("B", 3);  // Complete Graph
	struct nGraph P1 = newPath("P", 3);           // Path Graph
	struct nGraph T1 = newRandomTree("T", 10);    // Random Tree
	struct nGraph R1 = newErdosRenyiGNP("G", 50, .01); // Random Graph G(N,P)
	struct nGraph R2 = newErdosRenyiGNM("G", 50, 20);  // Random Graph G(N,M)
	struct nGraph R3 = newBarabasiAlbert("B", &T1, 20); // Random Scale Free Graph


	show(&S1);
	show(&K1);
	show(&P1);
	show(&T1);
	show(&R1);
	show(&R2);
	showDot(&R3);

	return 0;
}
