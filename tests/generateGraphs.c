#include "nucesGraph.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	struct nGraph G[11];

	G[0]  = newStar("S", 5);                 // Star Graph
	G[1]  = newCompleteGraph("B", 3);        // Complete Graph
	G[2]  = newPath("P", 3);                 // Path Graph
	G[3]  = newRing("C", 5);                 // Ring Graph 
	G[4]  = newRandomTree("T", 10);          // Random Tree
	G[5]  = newErdosRenyiGNP("G", 50, .01);  // Random Graph G(N,P)
	G[6]  = newErdosRenyiGNM("G", 50, 20);   // Random Graph G(N,M)
	G[7]  = newBarabasiAlbert("BB", &B, 30); // Random Scale Free Graph
	G[8]  = newHyperCube("G", 5);            // Hyper cube of order upto 5
	G[9]  = newPetersenGraph("P");           // Petersens Graph
	G[10] = newWheel("W", 21);               // Wheel Graph

	int i;
	for (i = 0; i < 11; i++) {
		show(&G[i]);
	}

	for (i = 0; i < 11; i++) {
		nGraphFree(&G[i]);
	}


	return 0;
}
