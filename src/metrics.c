#include "nucesGraph.h"

/**
 * Gets a list of common neighbors, given a graph \f$G\f$ and two of its 
 * vertices \f$u,v\f$.
 * @param G Graph object
 * @param u Vertex Label
 * @param v Vertex Label
 * @return List of Common Neighbors in form of a graph object
 */

struct nGraph getCommonNeighbors(struct nGraph *G, int first, int second)
{
	struct nGraph neighOne = getNeighbours(G, first);
	struct nGraph neighTwo = getNeighbours(G, second);

	struct nGraph common   = gIntersection(&neighOne, &neighTwo);
	return common;
}

/**
 * Gets similarity score in form of number of common neighbors, given a graph 
 * \f$G\f$ and two of its vertices \f$u,v\f$.
 * @param G Graph object
 * @param u Vertex label
 * @param v Vertex label
 * @return Similarity Score
 */
int commonNeighbors(struct nGraph *G, int u, int v)
{
	struct nGraph common = getCommonNeighbors(G, u, v);
	return common.V->count;
}


