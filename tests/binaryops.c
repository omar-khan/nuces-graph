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

	return 0;
}
