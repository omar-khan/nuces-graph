#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	/* Cross Product Check to Construct a Hypercube */
	struct nGraph L1 = newRing("L1", 4);
	struct nGraph L2 = newPath("L2", 2);

	struct nGraph L3 = crossProduct(&L1, &L2); // Cube
	show(&L3);
	showDot(&L3);

	struct nGraph L4 = crossProduct(&L3, &L2); // Hypercube
	show(&L4);
	showDot(&L4);


	/* Common Neighbor Check Test */

	struct nGraph home = newCompleteGraph("H",4);
	addVertex(&home, 4);
	addEdge(&home, 2, 4, 1);
	addEdge(&home, 3, 4, 1);

	showDot(&home);
	printf("%d Common Neighbors of %d and %d\n", commonNeighbors(&home, 2, 3), 2, 3);
	struct nGraph common = getCommonNeighbors(&home, 2, 3);
	show(&common);

	return 0;
}
