/**
 * @brief Code for generating families of graphs
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "nucesGraph.h"



/**
 * Constructs a random tree.
 * @param C Label for the tree
 * @param V Number of vertices required in the tree
 * @return Graph object
 */

struct nGraph newRandomTree(char *C, int V)
{
	struct nGraph tmp = newGraph(C);
	
	struct nGraph Done    = newGraph("D");
	struct nGraph NotDone = newGraph("N");

	int i;
	for (i = 0; i < V; i++) {
		addVertex(&NotDone, i);
	}

	srand(time(0));
	int randomHead = rand() % V;
	int randomTail = rand() % V;
	while (randomTail == randomHead) {
		randomTail = rand() % V;
	}
	addVertex(&Done, randomHead);
	addVertex(&Done, randomTail);
	addEdge(&Done, randomHead, randomTail, 0);

	removeVertex(&NotDone, randomHead);
	removeVertex(&NotDone, randomTail);

	while (Done.V->count != V) {
		
		randomHead = rand() % V;
		while (!searchVertex(&Done, randomHead)) {
			randomHead = rand() % V;
		}
		
		randomTail = rand() % V;
		while (!searchVertex(&NotDone, randomTail)) {
			randomTail = rand() % V;
		}

		addVertex(&Done, randomTail);
		addEdge(&Done, randomHead, randomTail, 0);
		removeVertex(&NotDone, randomTail);
	}

	copyGraph(&tmp, &Done);

	return tmp;
}

/**
 * Constructs a Complete Graph \f$K_{n}\f$ of \f$n\f$ vertices, where degree of each
 * vertex is \f$n-1\f$. 
 * @param c Graph Label
 * @param n Number of Vertices
 * @return Graph Object
 */

struct nGraph newCompleteGraph(char *c, int n)
{
	struct nGraph tmp = newGraph(c);
	
	int i, j;
	for (i = 0; i < n; i++) 
		addVertex(&tmp, i);
	for (i = 0; i < n; i++) {
		for (j = i; j < n; j++) {
			if (i != j) 
				addEdge(&tmp, i, j, 1);
		}
	}

	return tmp;
}

/**
 * Constructs a Star graph \f$S_{n}\f$ of n vertices, where the first vertex
 * \f$v_{0}\f$ has degree \f$n-1\f$, and the remaining are pendant vertices. 
 * @param c Graph label
 * @param n Number of Vertices
 * @return Graph object
 */

struct nGraph newStar(char *c, int n)
{
	struct nGraph tmp = newGraph(c);
	addVertex(&tmp, 0);
	int i;
	for (i = 1; i < n; i++) {
		addVertex(&tmp, i);
		addEdge(&tmp, 0, i, 1);
	}
	return tmp;
}

/**
 * Constructs a Path graph \f$P_{n}\f$ of n vertices
 * @param n Number of Vertices
 * @return Graph object
 */

struct nGraph newPath(char *c, int n)
{
	struct nGraph tmp = newGraph(c);
	
	int i;
	for (i = 0; i < n; i++) {
		addVertex(&tmp, i);
	}

	for (i = 0; i < n-1; i++) {
		addEdge(&tmp, i, i+1, 1);
	}

	return tmp;
}


