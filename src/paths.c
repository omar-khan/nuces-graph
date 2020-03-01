#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "nucesGraph.h"

struct nGraph shortestPath(struct nGraph *G, int start, int end)
{
	struct nGraph P = newGraph("P");

	/* To Do */

	return P;
}

int graphRadius(struct nGraph *G)
{
	int i, center = pow(2,31)-1, tmp;
	for (i = 0; i < G->V->count; i++) {
		tmp = eccentricity(G, i);
		if (tmp < center) 
			center = tmp;
	}
	return center;
}

int graphDiameter(struct nGraph *G)
{
	int i, center = 0, tmp;
	for (i = 0; i < G->V->count; i++) {
		tmp = eccentricity(G, i);
		if (tmp > center) 
			center = tmp;
	}
	return center;
}


struct nGraph graphCenter(struct nGraph *G) 
{
	/* Does not have support yet for edges of center vertices */
	struct nGraph C = newGraph("C");

	int i;
	for (i = 0; i < G->V->count; i++) {
		eccentricity(G, i);
	}

	int center = pow(2,31)-1;;
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->eccentricity < center) {
			center = tmp->eccentricity;
		}
		tmp = tmp->next;
	}

	tmp = G->V->head;
	while(tmp != NULL) {
		if (tmp->eccentricity == center) {
			addVertex(&C, tmp->label);
		}
		tmp = tmp->next;
	}

	return C;
}

int eccentricity(struct nGraph *G, int v) 
{
	int i, ecc = 0, ecctmp;
	for (i = 0; i < G->V->count; i++) {
		ecctmp = shortestPathLength(G, v, i);
		if (ecctmp > ecc) {
			ecc = ecctmp;
		}
	}
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->label == v) {
			tmp->eccentricity = ecc;
		}
		tmp = tmp->next;
	}
	return ecc;
}

/** Implementation of Dijkstra's shortest path algorithm. Assumed that all
 * vertices are in order from 0 ... n. For arbitrary vector labels, all arrays
 * need to be converted to linked lists. 
 *
 * Routine has not been tested thoroughly for edge weight zero. Assumes weights
 * are >= 1, where 1 represents one hop distance.
 *
 * @param G Graph object G=(V,E)
 * @param start Distance from
 * @param end Distance to
 * @return Length as an Integer number.
 */

int shortestPathLength(struct nGraph *G, int start, int end)
{
	struct nGraph Q = newGraph("Q");
	int distances[G->V->count];
//	int *distances = malloc(sizeof(int)*G->V->count);
	copyVertices(G, &Q);

	int i;
	for (i = 0; i < G->V->count; i++) {
		distances[i] = i == start ? 0 : pow(2,32)-1;
	}

	struct nGraph neigh;
	int selected, selectedDistance;
	int d = pow(2,31)-1;
	struct vertex *tmp;
	//int count = 0;
	while (Q.V->count != 0) {
		selected 		 = pow(2,31)-1;
		selectedDistance = pow(2,31)-1;
		/*printf("\n ---------\n");
		listVertices(&Q);
		printf("distances: ");
		for (i = 0; i < G->V->count; i++) {
			printf("%d, ", distances[i]);
		}
		printf("\n");*/
		for (i = 0; i < G->V->count; i++) {
			if (searchVertex(&Q, i)) {
				if (distances[i] < selectedDistance) {
					selectedDistance = distances[i];
					selected         = i;
				}
			}
		}
		//printf("selected node %d: distance = %d\n", selected, distances[selected]);
		removeVertex(&Q, selected);
		//listVertices(&Q);
		neigh = getNeighbours(G, selected);
		//nGraphInit(&neigh, "n");
		//placeNeighbours(G, selected, &neigh);
		//listVertices(&neigh);
		tmp = neigh.V->head;
		while (tmp != NULL) {
			if (searchVertex(&Q, tmp->label)) {
				d = distances[selected] + getEdgeWeight(G, tmp->label, selected);
		//		printf("distance: %d + %d\n", distances[selected], getEdgeWeight(G, selected, tmp->label));
				if (d < distances[tmp->label]) 
					distances[tmp->label] = d;
			}
			tmp = tmp->next;
		}
		//printf("distances: ");
		//for (i = 0; i < G->V->count; i++) {
		//	printf("%d, ", distances[i]);
		//}
		//printf("\n --------- \n");
		//count++;
		//if (count == 3) 
			//exit(0);
		nGraphFree(&neigh);
	}
	nGraphFree(&Q);
	free(tmp);
	return distances[end];
}


