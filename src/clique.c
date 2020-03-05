#include <unistd.h>
#include "nucesGraph.h"
#include <stdio.h>

/** 
 * This is the Classical Bron Kerbosch method for finding maximum / maximal cliques in a
 * graph. All cliques are displayed as output and are not returned as objects to
 * other parts of the algorithm. 
 * @param G Graph object containing original graph
 * @param P set containing vertices of G, represents possible  candidates
 * @param R an empty set representing temporary results
 * @param X an empty set representing excluded set
 * @param depth Tree depth. Specify zero (0). Only used for formatting output.
 */

void BronKerbosch(struct nGraph *G, struct nGraph *P, struct nGraph *R, struct nGraph *X)
{
	struct nGraph PunionX = gUnion(P, X);
	int maximum = 0;
	if (PunionX.V->count == 0) {
		printf("Maximal: ");
		listBK_temp(R);
		printf("\n");
		if (R->V->count > maximum) {
			maximum = R->V->count;
		}
		return;
	}

	struct nGraph neigh        = newGraph("N");
	struct nGraph PinterNeighV = newGraph("P");
	struct nGraph RunionV      = newGraph("R");
	struct nGraph XinterNeighV = newGraph("X");

	struct vertex *tmp = P->V->head;
	while(tmp != NULL) {
		neigh				 = getNeighbours(G, tmp->label); 
		PinterNeighV = gIntersection(P, &neigh);
		graphChangeLabel(&PinterNeighV, "P");
		RunionV   	 = gUnionVertex(R, tmp->label);
		graphChangeLabel(&RunionV, "R");
		XinterNeighV = gIntersection(X, &neigh);
		graphChangeLabel(&XinterNeighV, "X");
		BronKerbosch(G, &PinterNeighV, &RunionV, &XinterNeighV);

		*X = gUnionVertex(X, tmp->label);
		//addVertex(X, tmp->label);
		removeVertex(P, tmp->label);

		tmp = tmp->next;
	}
}


