#include <unistd.h>
#include "nucesGraph.h"

void BronKerbosch(struct nGraph G, struct nGraph P, struct nGraph R, struct nGraph X)
{
	#if 0
	listVertices(&P);
	listVertices(&R);
	listVertices(&X);

	printf("\n");
	#endif

	struct nGraph PunionX = gUnion(&P, &X);
	int maximum = 0;
	if (PunionX.V->count == 0) {
		listVertices(&R);
		if (R.V->count > maximum) {
			maximum = R.V->count;
		}
		return;
	}

	struct nGraph neigh;
	struct nGraph PinterNeighV;
	struct nGraph RunionV;
	struct nGraph XinterNeighV;

	struct vertex *tmp = P.V->head;
	while(tmp != NULL) {
		neigh 		 = getNeighbours(&G, tmp->label);
		PinterNeighV = gIntersection(&P, &neigh);
		RunionV   	 = gUnionVertex(&R, tmp->label);
		XinterNeighV = gIntersection(&X, &neigh);
		BronKerbosch(G, PinterNeighV, RunionV, XinterNeighV);

		removeVertex(&P, tmp->label);
		X = gUnion(&X, &neigh);

		tmp = tmp->next;
	}
}


