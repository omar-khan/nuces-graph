#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "nucesGraph.h"

const char *colors[20] = { "red", "green", "skyblue", "yellow", "white", "pink", "darkseagreen1", "magenta", "khaki", "grey", "gold", "orange", "aquamarine", "darkgreen", "deeppink", "greenyellow", "peachpuff", "slateblue", "turquoise", "violet"};
const int colornumbers[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 19};

/**
 * Frees Allocated Memory gracefully by first removing all vertex nodes, then
 * edge nodes, and then freeing their lists.
 * @param G Graph object
 */

void nGraphFree(struct nGraph *G) 
{
	while (G->V->count != 0) {
		if (G->V->tail->lblString != NULL)
			free(G->V->tail->lblString);
		removeVertexPopLast(G);
	}
	
	while(G->E->count != 0) {
		removeEdgePopLast(G);
	}

	free(G->label);
	free(G->V);
	free(G->E);

	if (G->adjacency_matrix != NULL) free(G->adjacency_matrix);
	if (G->incidence_matrix != NULL) free(G->incidence_matrix);
}

/** 
 * Change the Label of a Graph.
 * @param G Graph object
 * @param t New Label
 */

void graphChangeLabel(struct nGraph *G, char *t)
{
	G->label = calloc(sizeof(t)/sizeof(char), sizeof(char));
	strcpy(G->label, t);
}

/** 
 * Initializes internal pointers of a Graph object that has already been
 * created. Also sets various internal counters to zero.
 * @param G Graph object G=(V,E)
 * @param t Label to assign to the Graph (maximum 10 characters)
 */

void nGraphInit(struct nGraph *G, char *t)
{
	G->directed = 0;
	G->displayType = 0;
	G->V = calloc(1,sizeof(struct verList));//malloc(sizeof(struct verList));
	G->V->count = 0;
	G->E = calloc(1,sizeof(struct edgList));//malloc(sizeof(struct edgList));
	G->E->count = 0;
	G->label = calloc(strlen(t)+1,sizeof(char));//malloc(sizeof(char)*10);
	strncpy(G->label, t, sizeof(char)*strlen(t));
	G->V->deg_info.degree_histogram_in  = NULL;
	G->V->deg_info.degree_histogram_out = NULL;
	G->adjacency_matrix        = NULL;
	G->incidence_matrix        = NULL;
}

/**
 * Creates a new (Empty) Graph and calls relevant routines to initialize its
 * internal pointers.
 * @param c Label of the Graph
 * @return Graph Object
 */

struct nGraph newGraph(char *c)
{
	struct nGraph result;
	nGraphInit(&result, c);
	return result;
}

/**
 * Helper function for addVertexDuplicateOk() that counts the number of times a vertex appears in a list. For most algorithms, duplicate vertices are not supported and if run on any graph, it will return a 1. If used with addVertexDuplicateOK(), it can possibly return a count > 1. Function added to support finding of multinomial coefficients.
 * @param G Graph object
 * @param search Integer label of Vertex to search
 * @return Count of number of vertices found
 */

int searchVertexCount(struct nGraph *G, int search) 
{
	struct vertex *tmp = G->V->head;
	int count = 0;
	while (tmp != NULL) {
		if (tmp->label == search) {
			count++;
		}
		tmp = tmp->next;
	}
	return count;
}

/** 
 * Find a particular vertex in a graph.
 * @param G Graph object
 * @param search Vertex to search for
 * @return Not found = 0, Found = 1
 */

int searchVertex(struct nGraph *G, int search) 
{	
	if (G->V->count == 0) return 0;
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->label == search) {
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

void updateVertexDegree(struct nGraph *G, int vertex_from, int vertex_to, int directed, int add)
{
	struct vertex *tmp;
	if (directed == 0) {
		tmp = G->V->head;
		while (tmp != NULL) {
			if (tmp->label == vertex_from) {
				add == 1 ? tmp->degree_in++  : tmp->degree_in--;
				add == 1 ? tmp->degree_out++ : tmp->degree_out--;
				break;
			}
			tmp = tmp->next;
		}
	
		tmp = G->V->head;
		while (tmp != NULL) {
			if (tmp->label == vertex_to) {
				add == 1 ? tmp->degree_in++  : tmp->degree_in--;
				add == 1 ? tmp->degree_out++ : tmp->degree_out--;;
				break;
			}
			tmp = tmp->next;
		}
	} else {
		tmp = G->V->head;
		while (tmp != NULL) {
			if (tmp->label == vertex_from) {
				add == 1 ? tmp->degree_in++ : tmp->degree_in--;
				break;
			}
			tmp = tmp->next;
		}
	
		tmp = G->V->head;
		while (tmp != NULL) {
			if (tmp->label == vertex_to) {
				add == 1 ? tmp->degree_out++ : tmp->degree_out--;
				break;
			}
			tmp = tmp->next;
		}
	}
}

/** 
 * Get the assigned color (flag) of a given vertex
 * @param G Graph object
 * @param search Vertex to search
 * @return Color as Integer, or -1 if not found
 */

int getVertexColor(struct nGraph *G, int search) 
{	
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->label == search) {
			return tmp->color;
		}
		tmp = tmp->next;
	}
	return -1;
}

/** 
 * Each vertex is internally identified as an integer number. However, a label
 * for the vertex can also be specified. This searches for a given vertex by
 * it's label.
 * @param G Graph object
 * @param c Vertex label
 * @return Found = 1, Not Found = 0
 */

int searchVertexLabel(struct nGraph *G, char *c) 
{
	if (G->V->count == 0) return 0;
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->lblString != NULL) {
			if (strcmp(tmp->lblString, c) == 0) {
				return 1;
			}	
		}
		tmp = tmp->next;
	}
	return 0;
}

void copyVertexLabel(struct nGraph *G, int c, char *thelabel)
{
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->label == c && tmp->lblString != NULL) {
			strcpy(thelabel, tmp->lblString);
			break;
		}
	}
}

int getVertexLabel(struct nGraph *G, char *search)
{
	struct vertex *tmp = G->V->head;
	int count = 0; 
	while (tmp != NULL) {
		if (strcmp(tmp->lblString, search) == 0) {
			return count;
		}
		tmp = tmp->next;
		count++;
	}
	return 0;
}

int getVertex(struct nGraph *G, int search) 
{
	struct vertex *tmp = G->V->head;
	int count = 0; 
	while (tmp != NULL) {
		if (tmp->label == search) {
			return count;
		}
		tmp = tmp->next;
		count++;
	}
	return 0;
}

int getEdgeWeight(struct nGraph *G, int head, int tail) 
{
	struct edge *tmp = G->E->head;
	while (tmp != NULL) {
		if (tmp->directed == 0) {
			if (((tmp->head == head && tmp->tail == tail) || 
			     (tmp->head == head && tmp->tail == head)) ||
			    ((tmp->head == tail && tmp->tail == tail) ||
			 	 (tmp->head == tail && tmp->tail == head))) {
				return tmp->weight;
			}
		} else {
			if (tmp->head == head && tmp->tail == tail) {
				return tmp->weight;
			}
		}
		tmp = tmp->next;
	}
	return 0;
}

/** 
 * Search whether a given directed edge exists in a graph. Edges are specified as
 * head-tail pairs and must be specified in order.
 * @param G Graph object
 * @param head First incident vertex on edge
 * @param tail Second incident vertex on edge
 * @return Not Found = 0, Found = 1
 */
int directedEdgeExists(struct nGraph *G, int head, int tail) 
{
	struct edge *tmp = malloc(sizeof(struct edge));
	tmp = G->E->head;
	while (tmp != NULL) {
		if (tmp->head == head && tmp->tail == tail) {
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

/** 
 * Search whether a given edge exists in a graph. Edges are specified as
 * head-tail pairs. Order is irrelevant.
 * @param G Graph object
 * @param head First incident vertex on edge
 * @param tail Second incident vertex on edge
 * @return Not Found = 0, Found = 1
 */
int edgeExists(struct nGraph *G, int head, int tail) 
{
	struct edge *tmp = G->E->head;
	while (tmp != NULL) {
		if (((tmp->head == head && tmp->tail == tail) || 
		     (tmp->head == head && tmp->tail == head)) ||
		    ((tmp->head == tail && tmp->tail == tail) ||
		 	 (tmp->head == tail && tmp->tail == head))) {
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

/**
 * Add any random edge (uniform distribution) of a certain weight
 * @param G Graph object
 * @param weight Weight of the edge
 */

void addRandomEdge(struct nGraph *G, int weight)
{
	struct edge *tmp = malloc(sizeof(struct edge));
	tmp->label  = G->E->count;
	tmp->weight = weight;
	tmp->directed = 0;
	int randomHead = rand() % G->V->count;
	int randomTail = rand() % G->V->count;
	while (randomTail == randomHead) {
		randomTail = rand() % G->V->count;
	}
	tmp->head   = randomHead;
	tmp->tail   = randomTail;

	if (!edgeExists(G, tmp->head, tmp->tail)) {
		if (G->E->count == 0) {
			G->E->head = tmp;
			G->E->tail = tmp;
			G->E->count++;
		}
		else {
			G->E->tail->next = tmp;
			G->E->tail       = tmp;
			G->E->count++;
		}
		updateVertexDegree(G, tmp->head, tmp->tail, tmp->directed, 1);
	}
}

void addEdgeLabel(struct nGraph *G, char *head, char *tail, int weight)
{
	if (strcmp(head, tail) != 0) {
		int h, t;
		if (!searchVertexLabel(G, head)) {
			printf("invalid vertex %s in edge %s->%s\n", head, head, tail);
			return;
		} 
		else {
			h = getVertexLabel(G, head);
		}
		if (!searchVertexLabel(G, tail)) {
			printf("invalid vertex %s in edge %s->%s\n", tail, head, tail);
			return; 
		}
		else {
			t = getVertexLabel(G, tail);
		}
		addEdge(G, h, t, weight);
	}
}

void addEdgeDirected(struct nGraph *G, int head, int tail, int weight)
{
	if (head != tail) {
		if (!searchVertex(G, head)) {
			printf("invalid vertex %d in edge %d->%d\n", head, head, tail);
			return;
		}
		if (!searchVertex(G, tail)) {
			printf("invalid vertex %d in edge %d->%d\n", tail, head, tail);
			return; 
		}
		if (!directedEdgeExists(G, head, tail)) {
			struct edge *tmp = malloc(sizeof(struct edge));
			tmp->label    = G->E->count;
			tmp->weight   = weight;
			tmp->head     = head;
			tmp->tail     = tail;
			tmp->directed = 1;
			tmp->next     = NULL;
			tmp->prev     = G->E->count == 0 ? NULL : G->E->tail;
	
			G->directed   = 1;

			updateVertexDegree(G, head, tail, tmp->directed, 1);

			if (G->E->count == 0) {
				G->E->head = tmp;
				G->E->tail = tmp;
				G->E->count++;
			}
			else {
				G->E->tail->next = tmp;
				G->E->tail       = tmp;
				G->E->count++;
			}
		}
	}
}

void addEdge(struct nGraph *G, int head, int tail, int weight)
{
	if (head != tail) {
		if (!searchVertex(G, head)) {
			printf("invalid vertex %d in edge %d->%d\n", head, head, tail);
			return;
		}
		if (!searchVertex(G, tail)) {
			printf("invalid vertex %d in edge %d->%d\n", tail, head, tail);
			return; 
		}
		if (!edgeExists(G, head, tail)) {
			struct edge *tmp = malloc(sizeof(struct edge));
			tmp->label    = G->E->count; // may be problematic if edges removed.
			tmp->weight   = weight;
			tmp->head     = head;
			tmp->tail     = tail;
			tmp->directed = 0;
			tmp->next     = NULL;
			tmp->prev     = G->E->count == 0 ? NULL : G->E->tail;

			updateVertexDegree(G, head, tail, tmp->directed, 1);
	
			if (G->E->count == 0) {
				G->E->head = tmp;
				G->E->tail = tmp;
				G->E->count++;
			}
			else {
				G->E->tail->next = tmp;
				G->E->tail       = tmp;
				G->E->count++;
			}
		}
	}
}

void copyGraphDuplicateOK(struct nGraph *dest, struct nGraph *src)
{
	struct vertex *tmp = src->V->head;
	while(tmp != NULL) {
		addVertexDuplicateOK(dest, tmp->label);
		tmp = tmp->next;
	}
	struct edge *tmpE = src->E->head;
	while(tmpE != NULL) {
		if (tmpE->directed == 0) {
			addEdge(dest, tmpE->head, tmpE->tail, tmpE->weight);
		} else {
			addEdgeDirected(dest, tmpE->head, tmpE->tail, tmpE->weight);
		}
		tmpE = tmpE->next;
	}
}

void copyGraph(struct nGraph *dest, struct nGraph *src)
{
	struct vertex *tmp = src->V->head;
	while(tmp != NULL) {
		addVertex(dest, tmp->label);
		tmp = tmp->next;
	}
	struct edge *tmpE = src->E->head;
	while(tmpE != NULL) {
		if (tmpE->directed == 0) {
			addEdge(dest, tmpE->head, tmpE->tail, tmpE->weight);
		} else {
			addEdgeDirected(dest, tmpE->head, tmpE->tail, tmpE->weight);
		}
		tmpE = tmpE->next;
	}
}

void addVertexDuplicateOK(struct nGraph *G, int c)
{
	struct vertex *tmp = malloc(sizeof(struct vertex));
	tmp->label = c;
	tmp->eccentricity = 0;
	tmp->color = -1;
	tmp->prev = G->V->tail;

	if (G->V->count == 0) {
		G->V->head = tmp;
		G->V->tail = tmp;
		G->V->count++;
	}
	else {
		G->V->tail->next = tmp;
		G->V->tail       = tmp;
		G->V->count++;	
	}
}

void addVertexLabel(struct nGraph *G, char *c)
{
	if (!searchVertexLabel(G, c)) {
		// Vertex not found
		int currentVertices = G->V->count; 
		addVertex(G, currentVertices);
		setVertexLabel(G, currentVertices, c);
	}
}

void addVertex(struct nGraph *G, int c)
{
	if (!searchVertex(G, c)) {
		struct vertex *tmp = malloc(sizeof(struct vertex));
		tmp->label        = c;
		tmp->lblString    = NULL;
		tmp->color        = -1;
		tmp->eccentricity = 0;
		tmp->degree_in    = 0;
		tmp->degree_out   = 0;
		tmp->prev = G->V->tail;
		tmp->next = NULL;

		if (G->V->count == 0) {
			G->V->head = tmp;
			G->V->tail = tmp;
			G->V->count++;
		}
		else {
			G->V->tail->next = tmp;
			G->V->tail       = tmp;
			G->V->count++;	
		}
	}
}

void setVertexLabel(struct nGraph *G, int search, char *c)
{
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->label == search) {
			tmp->lblString = malloc(sizeof(char)*strlen(c));
			strcpy(tmp->lblString, c);
		}
		tmp = tmp->next;
	}
}

void copyVertices(struct nGraph *src, struct nGraph *dest)
{
	struct vertex *tmp = src->V->head;
	while(tmp != NULL) {
		//if (!searchVertex(dest, tmp->label)) {
			addVertex(dest, tmp->label);
		//}
		tmp = tmp->next;
	}
	free(tmp);
}

struct nGraph getVertices(struct nGraph *src)
{
	struct nGraph result;
	nGraphInit(&result, "P");
	copyVertices(src, &result);
	return result;
}

void placeNeighbours(struct nGraph *G, int label, struct nGraph *result)
{
	//if (result == NULL) nGraphInit(result, "n");
	/*else { 
		nGraphFree(result);
		nGraphInit(result, "n");
	}*/

	struct edge *tmpE = G->E->head;
	while(tmpE != NULL) {
		if (tmpE->head == label) {
			if (!searchVertex(result, tmpE->tail)) {
				addVertex(result, tmpE->tail);
			}
		}
		if (tmpE->tail == label) {
			if (!searchVertex(result, tmpE->head)) {
				addVertex(result, tmpE->head);
			}
		}
		tmpE = tmpE->next;
	}
	free(tmpE);
}

struct nGraph getNeighbours(struct nGraph *G, int label)
{
	struct nGraph result = newGraph("n");

	struct edge *tmpE = G->E->head;
	while(tmpE != NULL) {
		if (tmpE->head == label) {
			if (!searchVertex(&result, tmpE->tail)) {
				addVertex(&result, tmpE->tail);
			}
		}
		if (tmpE->tail == label) {
			if (!searchVertex(&result, tmpE->head)) {
				addVertex(&result, tmpE->head);
			}
		}
		tmpE = tmpE->next;
	}
	free(tmpE);
	return result;
}

void removeEdge(struct nGraph *P, int c, int d)
{
	struct edge *tmp = P->E->head;
	struct edge *pre = tmp;

	while(tmp != NULL) {
		if ((tmp->head == c && tmp->tail == d) || (tmp->head == d && tmp->tail == c)) {
			if (pre == P->E->head) { // first link
				pre = tmp->next;
				P->E->head = pre;
			} else {
				pre->next = tmp->next;
			}
			P->E->count--;
			updateVertexDegree(P, c, d, 0, 0);

			tmp = tmp->next;
		}
		else {
			pre = tmp;
			tmp = tmp->next;
		}
	}
}

/**
 * Remove a set of edges that are incident to a vertex. This function is
 * suitably called whenever a vertex c is deleted. It would then be necesssary
 * to remove all edges that are incident on this vertex c.
 * @param P Graph object
 * @param c Vertex to delete
 */

void removeEdges(struct nGraph *P, int c)
{
	struct edge *tmp;
	struct edge *tmp2;

	tmp = P->E->head;

	while(tmp != NULL) {
		if (tmp->head == c || tmp->tail == c) {
			if (tmp->prev == NULL) { // first node
				P->E->head = tmp->next;
				P->E->head->prev = NULL;
				P->E->count--;
				updateVertexDegree(P, tmp->head, tmp->tail, 0, 0);
				free(tmp);
				tmp = P->E->head->next;
			} else if (tmp->next == NULL) { // last node
				P->E->tail = tmp->prev;
				P->E->tail->next = NULL;
				P->E->count--;
				updateVertexDegree(P, tmp->head, tmp->tail, 0, 0);
				free(tmp);
				tmp = P->E->tail->next;
			} else { // middle node
				tmp2 = tmp;
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;
				P->E->count--;
				updateVertexDegree(P, tmp->head, tmp->tail, 0, 0);
				tmp = tmp->next;
				free(tmp2);
			}
		} else {
			tmp = tmp->next;
		}
	}
}

void removeSpecificEdge(struct nGraph *G, struct edge *e)
{
	if (e->prev == NULL && e->next != NULL) { // implies start vertex
		
	} else if (e->prev != NULL && e->next == NULL) { // implies last vertex
	} else { // implies middle vertex
	}
}

/**
 * Prunes a graph by clipping away some edges with given Probability. 
 * @param G Graph object
 * @param P Probability
 */
void removeRandomEdges(struct nGraph *G, double P)
{
	if (G->E->count == 0) {
		return;
	} else {
		struct edge *tmp = G->E->head;
		struct edge *pre = NULL;
		double rnd;
		while (tmp != NULL) {
			rnd = rand()/(double)RAND_MAX;
			if (rnd <= P) { // removal
				if (tmp == G->E->head) { // first node
					if (G->E->count == 1) {
						updateVertexDegree(G, tmp->head, tmp->tail, 0, 0);
						G->E->count--;
						free(tmp);
						G->E->head = G->E->tail = tmp = NULL;
					} else {
						G->E->head = tmp->next;
						G->E->head->prev = NULL;
						G->E->count--;
						updateVertexDegree(G, tmp->head, tmp->tail, 0, 0);
						free(tmp);
						tmp = G->E->head;
					}
				} else if (tmp == G->E->tail) { // last node
					G->E->tail = tmp->prev;
					G->E->count--;
					updateVertexDegree(G, tmp->head, tmp->tail, 0, 0);
					free(tmp);
					tmp = G->E->tail;
					tmp->next = NULL;
					tmp = tmp->next;
				} else { // middle node
					tmp->prev->next = tmp->next;
					tmp->next->prev = tmp->prev;
					pre = tmp;
					G->E->count--;
					updateVertexDegree(G, tmp->head, tmp->tail, 0, 0);
					tmp = tmp->next;
					free(pre);
				}
			} else { // no removal
				tmp = tmp->next;
			}
		}
	}
}

void removeEdgePopLast(struct nGraph *P)
{
	if (P->E->count == 1) {
		struct edge *tmp = P->E->tail;
		P->E->tail = NULL;
		free(tmp);
		P->E->count--;
	}
	else {
		struct edge *tmp = P->E->tail;
		P->E->tail = tmp->prev;
		P->E->tail->next = NULL;
		free(tmp);
		P->E->count--;
	}
}

void removeVertexPopLast(struct nGraph *P)
{
	if (P->V->count == 1) {
		struct vertex *tmp = P->V->tail;
		P->V->tail = NULL;
		free(tmp);
		P->V->count--;
	}
	else {
		struct vertex *tmp = P->V->tail;
		P->V->tail = tmp->prev;
		P->V->tail->next = NULL;
		free(tmp);
		P->V->count--;
	}
}

/**
 * Removes a vertex from a graph, and consequently, all incident edges on the
 * vertex.
 * @param P Graph object
 * @param c Vertex label
 */

void removeVertex(struct nGraph *P, int c)
{
	struct vertex *tmp = P->V->head;
	struct vertex *pre;

	if (P->V->count != 0) {
		if (P->V->head->label == c) {
			P->V->head = P->V->head->next;
			P->V->count--;
			removeEdges(P, c);
			free(tmp);
		}
		else {
			pre = tmp;
			tmp = tmp->next;
			while(tmp != NULL) {
				if (tmp->label == c) {
					pre->next = tmp->next;
					P->V->count--;
					removeEdges(P, c);
					free(tmp);
					break;
				}
				else {
					pre = tmp;
					tmp = tmp->next;
				}
			}
		}
	}
}

/**
 * Generates the adjacency matrix for a graph. The adjacency matrix is not
 * implicitly calculated whenever a graph topology is changed. It must be
 * explicitly called in order to generate the matrix. Internally, the adjacency
 * matrix is stored as a 1D array.
 * @param G Graph object
 */

void adjacencyMatrix(struct nGraph *G)
{
	int i,j;
	if (G->adjacency_matrix != NULL) {
		free(G->adjacency_matrix);
	}

	G->adjacency_matrix = calloc(G->V->count*G->V->count, sizeof(int));

	if (G->V->tail->label != G->V->count-1) {
	}

	struct edge *tmp = G->E->head;
	while (tmp != NULL) {

		if (tmp->directed == 0) {
			G->adjacency_matrix[tmp->head*G->V->count+tmp->tail] = 1;
			G->adjacency_matrix[tmp->tail*G->V->count+tmp->head] = 1;
		} else {
			G->adjacency_matrix[tmp->head*G->V->count+tmp->tail] = 1;
		}

		tmp = tmp->next;
	}
	printf("A(%s):", G->label);
	for (j = 0; j < G->V->count; j++) {
		printf("\t");
		for (i = 0; i < G->V->count; i++) {
			printf("%d ", G->adjacency_matrix[j*G->V->count+i]);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Generate the incidence matrix for a graph. The incidence matrix is not
 * implicitly calculated whenever a graph topology is changed. It must be
 * explicitly called in order to generate the matrix. Internally, the incidence
 * matrix is stored as a 1D array.
 * @param G Graph object
 */

void incidenceMatrix(struct nGraph *G)
{
	int i,j;
	if (G->incidence_matrix != NULL) {
		free(G->incidence_matrix);
	}

	G->incidence_matrix = calloc(G->V->count*G->E->count, sizeof(int));

	struct edge *tmp = G->E->head;
	while (tmp != NULL) {
		if (tmp->directed == 0) {
			G->incidence_matrix[tmp->label*G->V->count+tmp->head] = 1;
			G->incidence_matrix[tmp->label*G->V->count+tmp->tail] = 1;
		} else {
			G->incidence_matrix[tmp->label*G->V->count+tmp->head] = 1;
			G->incidence_matrix[tmp->label*G->V->count+tmp->tail] = -1;
		}

		tmp = tmp->next;
	}

	printf("I(%s):", G->label);
	for (j = 0; j < G->E->count; j++) {
		printf("\t");
		for (i = 0; i < G->V->count; i++) {
			printf("%d ", G->incidence_matrix[j*G->V->count+i]);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Prints the histogram of vertex degrees in a graph. Both in-degree and
 * out-degree are determined and printed.
 * @param G Graph object
 */

void degreeHistogram(struct nGraph *G)
{
	int min_in_degree = 1000, min_out_degree = 1000;
	int max_in_degree = 0,    max_out_degree = 0;
	int sum_in_degree = 0,    sum_out_degree = 0;
	double avg_in_degree,        avg_out_degree;
	struct vertex *tmp = G->V->head;
	while (tmp != NULL) {
		if (tmp->degree_in  > max_in_degree)   max_in_degree  = tmp->degree_in;
		if (tmp->degree_in  <= min_in_degree)  min_in_degree  = tmp->degree_in;
		if (tmp->degree_out > max_out_degree)  max_out_degree = tmp->degree_out;
		if (tmp->degree_out <= min_out_degree) min_out_degree = tmp->degree_out;
		sum_in_degree  += tmp->degree_in;
		sum_out_degree += tmp->degree_out;

		tmp = tmp->next;
	}
	avg_in_degree  = sum_in_degree / (double)G->V->count;
	avg_out_degree = sum_out_degree / (double)G->V->count;

	int range_in  = max_in_degree  - min_in_degree  + 1;
	int range_out = max_out_degree - min_out_degree + 1;

	G->V->deg_info.degree_histogram_in  = malloc(sizeof(int*)*(unsigned int)range_in);
	G->V->deg_info.degree_histogram_out = malloc(sizeof(int*)*(unsigned int)range_out);
	int i;
	for (i = 0; i < range_in; i++) {
		G->V->deg_info.degree_histogram_in[i] = malloc(sizeof(int)*2);
		G->V->deg_info.degree_histogram_in[i][0] = i+min_in_degree;
		G->V->deg_info.degree_histogram_in[i][1] = 0;
	}
	for (i = 0; i < range_out; i++) {
		G->V->deg_info.degree_histogram_out[i] = malloc(sizeof(int)*2);
		G->V->deg_info.degree_histogram_out[i][0] = i+min_out_degree;
		G->V->deg_info.degree_histogram_out[i][1] = 0;
	}

	tmp = G->V->head;
	while(tmp != NULL) {
		G->V->deg_info.degree_histogram_in[tmp->degree_in - min_in_degree][1]++;
		G->V->deg_info.degree_histogram_out[tmp->degree_out - min_out_degree][1]++;

		tmp = tmp->next;
	}
	if(G->directed == 1) {
		printf("Degree In Sum: %d, Mean: %f)\n", sum_in_degree, avg_in_degree);
		for (i = 0; i < range_in; i++) {
			printf("%d : %d\n", G->V->deg_info.degree_histogram_in[i][0], 
					                G->V->deg_info.degree_histogram_in[i][1]);
		}
		printf("Degree Out (Sum: %d, Mean: %f)\n", sum_out_degree, avg_out_degree);
		for (i = 0; i < range_out; i++) {
			printf("%d : %d\n", G->V->deg_info.degree_histogram_out[i][0],
													G->V->deg_info.degree_histogram_out[i][1]);
		}
	} else {
		printf("Degree (Sum: %d, Mean: %f)\n", sum_in_degree, avg_in_degree);
		for (i = 0; i < range_out; i++) {
			printf("%d : %d\n", G->V->deg_info.degree_histogram_in[i][0],
													G->V->deg_info.degree_histogram_in[i][1]);
		}
	}
	printf("\n");

	G->V->deg_info.sum_in_degree  = sum_in_degree;
	G->V->deg_info.sum_out_degree = sum_out_degree;
	G->V->deg_info.avg_in_degree  = avg_in_degree;
	G->V->deg_info.avg_out_degree = avg_out_degree;
	G->V->deg_info.range_in       = range_in;
	G->V->deg_info.range_out      = range_out;
	G->V->deg_info.max_in_degree  = max_in_degree;
	G->V->deg_info.min_in_degree  = min_in_degree;
	G->V->deg_info.max_out_degree = max_out_degree;
	G->V->deg_info.min_out_degree = min_out_degree;
}

int factorial(int f)
{
	if (f == 0) return 1;
	if (f == 1) return 1;
	return f * factorial(f-1);
}

int NcR(int n, int r)
{
	if (r == 0) return 1;
	if (r > n / 2) return NcR(n, n - r);
	long res = 1;
	int k;
	for (k = 1; k <= r; ++k) {
		res *= n - k + 1;
		res /= k;
	}
	return res;
}
