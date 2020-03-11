#include <stdlib.h>
#include <math.h>
#include "nucesGraph.h"

/**
 * Gets a list of common neighbors, given a graph \f$G\f$ and two of its 
 * vertices \f$u,v\f$.
 * @param G Graph object
 * @param u First Vertex Label
 * @param v Second Vertex Label
 * @return List of Common Neighbors in form of a graph object
 */

struct nGraph getCommonNeighbors(struct nGraph *G, int u, int v)
{
	struct nGraph neighOne = getNeighbours(G, u);
	struct nGraph neighTwo = getNeighbours(G, v);

	struct nGraph common   = gIntersection(&neighOne, &neighTwo);
	return common;
}

/**
 * Gets similarity score in form of number of common neighbors, given a graph 
 * \f$G\f$ and two of its vertices \f$u,v\f$.
 * @param G Graph object
 * @param u First Vertex label
 * @param v Second Vertex label
 * @return Similarity Score
 */
int commonNeighbors(struct nGraph *G, int u, int v)
{
	struct nGraph common = getCommonNeighbors(G, u, v);
	return common.V->count;
}

/**
 * Returns Jaccard Similarity coefficient for two vertices in a graph
 * @param G Graph object
 * @param u First Vertex label
 * @param v Second Vertex label
 * @return Jaccard Coefficient
 */

double jaccardCoefficient(struct nGraph *G, int u, int v)
{
	struct nGraph neighOne = getNeighbours(G, u);
	struct nGraph neighTwo = getNeighbours(G, v);

	struct nGraph neighIntersection = gIntersection(&neighOne, &neighTwo);
	struct nGraph neighUnion        = gUnion(&neighOne, &neighTwo);

	return (neighIntersection.V->count/(double)neighUnion.V->count);
}

/**
 * Returns Adamic-Adar Index for two vertices in a graph.
 * @param G Graph object
 * @param u First vertex label
 * @param v Second vertex label
 * @return Adamic-Adar Index
 */

double adamicAdarIndex(struct nGraph *G, int u, int v)
{
	struct nGraph neighOne = getNeighbours(G, u);
	struct nGraph neighTwo = getNeighbours(G, v);

	struct nGraph common   = gIntersection(&neighOne, &neighTwo);

	struct vertex *tmp = common.V->head;
	double sum = 0;
	struct nGraph common_temp;

	while(tmp != NULL) {
		common_temp = getNeighbours(G, tmp->label);
		sum += 1/(double)log(common_temp.V->count);
	
		tmp = tmp->next;
	}
	return sum;
}
