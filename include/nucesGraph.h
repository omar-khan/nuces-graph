/**
 * @file nucesGraph.h
 * @author Omar Usman Khan
 * @brief Header file linking everything together in the library
 */

#ifndef NUCESGRAPH_H
#define NUCESGRAPH_H

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
extern const char *colors[20];
extern const int  colornumbers[20];

struct vertex
{
	int label;
	char *lblString;
	int color;
	int eccentricity;
	int degree_in;
	int degree_out;
	struct vertex *next;
	struct vertex *prev;
};

struct edge 
{
	int label;
	int weight;
	int head;
	int tail;
	int directed;
	struct edge *next;
	struct edge *prev;
};

struct verList
{
	int count;
	struct vertex *head;
	struct vertex *tail;

	int **degree_histogram_in;
	int **degree_histogram_out;
};

struct edgList
{
	int count;
	struct edge *head;
	struct edge *tail;
};

typedef struct nGraph
{
	char *label;
	struct verList *V;
	struct edgList *E;
	int directed;

	int *adjacency_matrix;
	int *incidence_matrix;
} nGraph;

struct nGraph newGraph(char *);
void nGraphInit(struct nGraph *, char *);
void addVertex(struct nGraph *, int);
void addVertexLabel(struct nGraph *, char *);
void copyVertexLabel(struct nGraph *, int, char *);
void addVertexDuplicateOK(struct nGraph *, int);
void show(struct nGraph *);
void addEdge(struct nGraph *, int, int, int);
void addEdgeDirected(struct nGraph *, int, int, int);
void addEdgeLabel(struct nGraph *, char *, char *, int);
void showDot(struct nGraph *);
void exportDot(struct nGraph *);
void showDimac(struct nGraph *);
void exportDimac(struct nGraph *);
void makeDimac(struct nGraph *, char *);
int getVertexColor(struct nGraph *, int);
int searchVertex(struct nGraph *, int);
int searchVertexLabel(struct nGraph *, char *);
int eccentricity(struct nGraph *, int);
int edgeExists(struct nGraph *, int, int);
int shortestPathLength(struct nGraph *, int, int);
int getEdgeWeight(struct nGraph *, int, int);
int graphRadius(struct nGraph *);
int graphDiameter(struct nGraph *);
struct nGraph getVertices(struct nGraph *);
void makeRandomTree(struct nGraph *, int);
void printPermutations(struct nGraph);
void listAllTrees(struct nGraph *, int);
void addRandomEdge(struct nGraph *, int);
void BronKerbosch(struct nGraph, struct nGraph, struct nGraph, struct nGraph);
void removeVertex(struct nGraph *, int);
void removeVertexPopLast(struct nGraph *);
void removeEdgePopLast(struct nGraph *);
void listVerticesAlphabet(struct nGraph *);
void listVertices(struct nGraph *);
void listEdges(struct nGraph *);
void copyGraphDuplicateOK(struct nGraph *, struct nGraph *);
void copyGraph(struct nGraph *, struct nGraph *);
void copyVertices(struct nGraph *, struct nGraph *);
int searchVertexCount(struct nGraph *, int);
void setVertexLabel(struct nGraph *, int, char *);
struct nGraph newCompleteGraph(char *, int);
struct nGraph graphCenter(struct nGraph *);
struct nGraph gUnion(struct nGraph *, struct nGraph *);
struct nGraph gIntersection(struct nGraph *, struct nGraph *);
struct nGraph getNeighbours(struct nGraph *, int);
void placeNeighbours(struct nGraph *, int, struct nGraph *);
struct nGraph newCenterGraph(struct nGraph *);
struct nGraph gRingSum(struct nGraph *, struct nGraph *);
struct nGraph crossProduct(struct nGraph *, struct nGraph *);
struct nGraph gUnionVertex(struct nGraph *, int);
struct nGraph newPath(char *, int);
struct nGraph newStar(char *, int);
void makePath(struct nGraph *, int);
void nGraphFree(struct nGraph *);
void printCombinations(struct nGraph, int, int, int);
void degreeHistogram(struct nGraph *);
void adjacencyMatrix(struct nGraph *);
void incidenceMatrix(struct nGraph *);
#endif
