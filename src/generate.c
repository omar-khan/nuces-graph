/**
 * @brief Code for generating families of graphs
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "nucesGraph.h"

int factorial(int f)
{
	if (f == 0) return 1;
	if (f == 1) return 1;
	return f * factorial(f-1);
}

int multinomialCoefficient(struct nGraph *T, int vertices)
{
	int numerator = factorial(vertices - 2);
	int denominator = 1;
	int i;
	for (i = 0; i < vertices; i++) {
		denominator *= factorial(searchVertexCount(T, i));
	}
	return (numerator / denominator);
}

int totalMultinomialCoefficients(int vertices)
{
	int numerator = vertices + vertices - 2 - 1;
	int denominator = vertices - 1;
	return (factorial(numerator)/factorial(numerator-denominator)/factorial(denominator));
}

void swap(struct nGraph *T, int one, int two)
{
	/* This code only swaps label values of two positions 
	 * Does not actually change the positions of the vertices in the data
	 * structure
	 * WARNING: EDGES ARE NOT TOUCHED
	 */

	struct vertex *tmp1 = T->V->head;
	int count1 = 0, count2 = 0;
	if (searchVertex(T, one)) {
		while (tmp1 != NULL) {
			if (count1 == one) {
				printf("pos1 = %c %d\n", 'a' + tmp1->label, one);
				break;
			}
			tmp1 = tmp1->next;
			count1++;
		}
	} else { 
		//printf("one not found\n"); 
		return;
	}

	struct vertex *tmp2 = T->V->head;
	if (searchVertex(T, two)) {
		while (tmp2 != NULL) {
			if (count2 == two) {
				printf("pos2 = %c %d\n", 'a' + tmp2->label, two);
				break;
			}
			tmp2 = tmp2->next;
			count2++;
		}
	} else { 
		//printf("two not found\n"); 
		return;
	}
	
	printf("shuffle bef = %c %c\n", 'a' + tmp1->label, 'a' + tmp2->label);

	char replace = tmp1->label;
	tmp1->label = tmp2->label;
	tmp2->label = replace;

	printf("shuffle aft = %c %c\n", 'a' + tmp1->label, 'a' + tmp2->label);

}

void printPermutations(struct nGraph B)
{
	/* Make a bckup */
	struct nGraph T = newGraph("T");
	copyGraphDuplicateOK(&T, &B);

	/* Copy the Linked List to an Array */
	int i = 0;
	int *c = malloc(sizeof(int)*T.V->count);
	int *array = malloc(sizeof(int)*T.V->count);
	int n = T.V->count;

	struct vertex *tmp = T.V->head;
	while(tmp != NULL) 
	{
		array[i] = tmp->label;
		c[i]     = 0;
		tmp = tmp->next;
		i++;
	}
	int j;
	
	listVertices(&T);

	for (j = 0; j < n; j++) {
		removeVertex(&T, array[j]);
	}
	
	i = 0;
	int tempStorage;
	while (i < n) {
		if ( c[i] < i ) {
			if (i % 2 == 0) {
				tempStorage = array[0];
				array[0] = array[i];
				array[i] = tempStorage;
			}
			else {
				tempStorage = array[i];
				array[i] = array[c[i]];
				array[c[i]] = tempStorage;
			}
			for (j = 0; j < n; j++) {
				addVertexDuplicateOK(&T, array[j]);
			}
			listVertices(&T);
			for (j = 0; j < n; j++) {
				removeVertex(&T, array[j]);
			}

			c[i]++;
			i = 0;
		}
		else {
			c[i] = 0;
			i++;
		}
	}
}
void printPermutationsRecursvie(struct nGraph T, int N)
{
	
}

void printCombinations(struct nGraph T, int p, int start, int vertices) 
{
	int i;
	if (p == 0) {
		int coefficient = multinomialCoefficient(&T, vertices);
		printf("Multinomial Coefficient = %5d, ", coefficient);
		//listVerticesAlphabet(&T);				
		listVertices(&T);
		if (coefficient > 1) {
			//printPermutations(T);
			printPermutationsRecursvie(T, vertices);
		}
		return;
	}
	for (i = start; i < vertices; i++) {
		addVertexDuplicateOK(&T, i);
		printCombinations(T, p-1, i, vertices);
		removeVertexPopLast(&T);
	}
}

void listAllTrees(struct nGraph *G, int vertices)
{	
	int power = vertices - 2;
	struct nGraph T = newGraph("T");
	int start = 0;
	int totalCoefficients = totalMultinomialCoefficients(vertices);
	printf("Total Multinomial Combinations: %d\n", totalCoefficients);
	printCombinations(T, power, start, vertices);
}

/**
 * Constructs a random tree.
 * @param C Label for the tree
 * @param V Number of vertices required in the tree
 * @return Graph object
 */

struct nGraph newRandomTree(char *C, int V)
{
	struct nGraph tmp = newGraph(c);
	
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


