#include "nucesGraph.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	struct nGraph G[12];

	G[0]  = newStar("S", 5);                 // Star Graph
	G[1]  = newCompleteGraph("B", 3);        // Complete Graph
	G[2]  = newPath("P", 3);                 // Path Graph
	G[3]  = newRing("C", 5);                 // Ring Graph 
	G[4]  = newRandomTree("T", 10);          // Random Tree
	G[5]  = newRandomBinaryTree("T", 10);    // Random Binary Tree
	G[6]  = newErdosRenyiGNP("G", 50, .01);  // Random Graph G(N,P)
	G[7]  = newErdosRenyiGNM("G", 50, 20);   // Random Graph G(N,M)
	G[8]  = newBarabasiAlbert("BB", &G[2], 30); // Random Scale Free Graph
	G[9]  = newHyperCube("G", 9);            // Hyper cube of order upto 5
	G[10] = newPetersenGraph("P");           // Petersens Graph
	G[11] = newWheel("W", 21);               // Wheel Graph

	int i;
	for (i = 0; i < 12; i++) {
		showDot(&G[i]);
	}

	for (i = 0; i < 12; i++) {
		nGraphFree(&G[i]);
	}

	return 0;
}
