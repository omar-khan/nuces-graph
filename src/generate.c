/**
 * @brief This file contains code snippets for generating various families of
 * graphs.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/times.h>
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
 * @param c Label for the Graph
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

/**
 * Constructs a Ring graph \f$P_{n}\f$ of n vertices
 * @param c Label for the Graph
 * @param n Number of Vertices
 * @return Graph object
 */

struct nGraph newRing(char *c, int n)
{
	struct nGraph tmp = newPath(c, n);
	addEdge(&tmp, 0, tmp.V->count-1, 1);
	return tmp;
}

/**
 * Constructs a random graph (may be disconnected), given \f$N\f$ nodes. An edge
 * between unique pairs \f$i, j\f$ will be added to the graph based on its
 * probability \f$P\f$. The working principle follows the Erdos-Renyi G(N,P) 
 * Random Graph model.
 * @param lbl Label for the Graph
 * @param N number of vertices
 * @param P probability of an edge
 * @return Graph object
 */
struct nGraph newErdosRenyiGNP(char *lbl, int N, double P)
{
	struct nGraph tmp = newGraph(lbl);

	int i, j;
	for (i = 0; i < N; i++) {
		addVertex(&tmp, i);
	}

	double rnd;
	srand(clock());
	for (j = 0; j < N; j++) {
		for (i = j+1; i < N; i++) {
			if (i != j) {
				rnd = rand()/(double)RAND_MAX;
				if (rnd < P) { 
					addEdge(&tmp, i, j, 1);
				}
			}
		}
	}
	return tmp;
}

/**
 * Constructs a random graph (may be disconnected), given \f$N\f$ nodes. An edge
 * between unique pairs \f$i, j\f$ will be added to the graph based on its
 * probability \f$P\f$. The working principle follows the Erdos-Renyi G(N,M) 
 * Random Graph model.
 * @param lbl Label for the Graph
 * @param N number of vertices
 * @param M number of edges
 * @return Graph object
 */
struct nGraph newErdosRenyiGNM(char *lbl, int N, int M)
{
	struct nGraph tmp = newGraph(lbl);

	int i, j;
	for (i = 0; i < N; i++) {
		addVertex(&tmp, i);
	}

	double P = M/(double)((N*N-1)/2.);
	int count = 0;

	double rnd;
	srand(clock());

	while (count < M) {
		i = rand() % N;
		j = rand() % N;
		while (i == j) {
			j = rand() % N;
		}
		rnd = rand()/(double)RAND_MAX;
		if (rnd < P) {
			count++;
			addEdge(&tmp, i, j, 1);
		}
	}
	return tmp;
}

/**
 * Constructs a random scale free graph based on the Albert Barabasi model. To
 * construct the graph, a seed in the form of a smaller graph is required. 
 * @param lbl Label of the Graph
 * @param pattern Seed graph object
 * @param M number of additional vertices in the graph
 * @return Graph object
 */

struct nGraph newBarabasiAlbert(char *lbl, struct nGraph *pattern, int M)
{
	struct nGraph tmp = newGraph(lbl);
	copyGraph(&tmp, pattern);

	int i, count = 0;
	double P, rnd;
	struct vertex *tmp_vertex;

	i = 0;
	while(i < M) { 
		tmp_vertex = tmp.V->head;
		count      = 0; 
		while (count < pattern->V->count && tmp_vertex != NULL) {
			P = tmp_vertex->degree_in / (double)(2*tmp.E->count);
			rnd = rand()/(double)RAND_MAX;
			if (rnd < P) {
				addVertex(&tmp, tmp.V->count);
				addEdge(&tmp, tmp.V->count-1, tmp_vertex->label, 1);
				count++;
				i++;
			}
			tmp_vertex = tmp_vertex->next;
		}
	}
	return tmp;
}

/**
 * Returns a hypercube of a given order. At the moment there is a buffer
 * overflow error for orders > 3.
 * @param lbl Label for the Hypercube graph
 * @param order Order of the Hypercube graph
 * @return Graph object
 */

struct nGraph newHyperCube(char *lbl, int order)
{
	struct nGraph tmp = newPath(lbl, 2);
	struct nGraph P2  = newPath(lbl, 2);

	int i;
	for (i = 1; i < order; i++) {
		tmp = crossProduct(&tmp, &P2);
	}
	return tmp;
}

/**
 * Returns Petersen's graph of 10 vertices and 15 edges
 * @param lbl Label for the graph
 * @return Graph object
 */

struct nGraph newPetersenGraph(char *lbl)
{
	struct nGraph tmp = newGraph(lbl);
	int i;
	for (i = 0; i < 5; i++) {
		addVertex(&tmp, i);
		addVertex(&tmp, i+5);
	}
	for (i = 0; i < 10; i+=2) {
		addEdge(&tmp, i%5,   (i+2)%5, 1); // Inner Star
		addEdge(&tmp, i/2+5, (i/2+6)%5+5, 1);  
		addEdge(&tmp, i/2,   (i/2)+5, 1);
	}
	return tmp;
}

/* Returns a Wheel Graph of \f$n \geq 4\f$ vertices.
 * @param lbl Label for the graph
 * @param n Number of vertices
 * @return Graph object
 */

struct nGraph newWheel(char *lbl, int n)
{
	struct nGraph tmp = newGraph(lbl);

	int i;
	for (i = 0; i < n; i++) {
		addVertex(&tmp, i);
	}

	for (i = 1; i < n; i++) {
		addEdge(&tmp, i, i == n-1 ? 1 : i+1, 1);
		addEdge(&tmp, 0, i, 1);
	}

	return tmp;
}
