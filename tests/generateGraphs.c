#include "nucesGraph.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	struct nGraph S1 = newStar("S", 5);                 // Star Graph
	struct nGraph K1 = newCompleteGraph("B", 3);        // Complete Graph
	struct nGraph P1 = newPath("P", 3);                 // Path Graph
	struct nGraph C1 = newRing("C", 5);                 // Ring Graph 
	struct nGraph T1 = newRandomTree("T", 10);          // Random Tree
	struct nGraph R1 = newErdosRenyiGNP("G", 50, .01);  // Random Graph G(N,P)
	struct nGraph R2 = newErdosRenyiGNM("G", 50, 20);   // Random Graph G(N,M)
	struct nGraph R3 = newBarabasiAlbert("B", &T1, 20); // Random Scale Free Graph
	struct nGraph H1 = newHyperCube("G", 5);            // Hyper cube of order upto 5
	struct nGraph P2 = newPetersenGraph("P");           // Petersens Graph
	struct nGraph W1 = newWheel("W", 101);               // Wheel Graph

	show(&S1);
	show(&K1);
	show(&P1);
	show(&C1);
	show(&T1);
	show(&R1);
	show(&R2);
	show(&R3);
	show(&H1);
	show(&P2);
	showDot(&W1);
	
	return 0;
}
