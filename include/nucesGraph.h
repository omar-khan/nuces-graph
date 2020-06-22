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

struct degree_info
{
	int sum_in_degree;
	int sum_out_degree;
	int avg_in_degree;
	int avg_out_degree;
	int range_in;
	int range_out;
	int max_in_degree;
	int min_in_degree;
	int max_out_degree;
	int min_out_degree;
	int **degree_histogram_in;
	int **degree_histogram_out;
};

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
	struct degree_info deg_info;
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
	int displayType;
	int *adjacency_matrix;
	int *incidence_matrix;
} nGraph;

// Basic Signatures
struct nGraph newGraph(char *);
void nGraphInit(struct nGraph *, char *);
void addVertex(struct nGraph *, int);
void addVertexLabel(struct nGraph *, char *);
void copyVertexLabel(struct nGraph *, int, char *);
void graphChangeLabel(struct nGraph *, char *);
void addVertexDuplicateOK(struct nGraph *, int);
void addEdgeLabel(struct nGraph *, char *, char *, int);
void addEdgeDirected(struct nGraph *, int, int, int);
void addEdge(struct nGraph *, int, int, int);
void addRandomEdge(struct nGraph *, int);
void removeEdge(struct nGraph *, int, int);
void removeVertex(struct nGraph *, int);
void removeVertexPopLast(struct nGraph *);
void removeEdgePopLast(struct nGraph *);
void removeRandomEdges(struct nGraph *, double);
void copyGraphDuplicateOK(struct nGraph *, struct nGraph *);
void copyGraph(struct nGraph *, struct nGraph *);
void copyVertices(struct nGraph *, struct nGraph *);
void nGraphFree(struct nGraph *);
void adjacencyMatrix(struct nGraph *);
void incidenceMatrix(struct nGraph *);

// Display related
void show(struct nGraph *);
void showDot(struct nGraph *);
void exportDot(struct nGraph *);
void showDimac(struct nGraph *);
void exportDimac(struct nGraph *);
void makeDimac(struct nGraph *, char *);
void exportGLPK(struct nGraph *);
void listVerticesAlphabet(struct nGraph *);
void listVertices(struct nGraph *);
void listBK_temp(struct nGraph *);
void listEdges(struct nGraph *);
void setDisplayType(struct nGraph *, char *);

// Search 
int searchVertexLabel(struct nGraph *, char *);
int searchVertex(struct nGraph *, int);
int edgeExists(struct nGraph *, int, int);
int searchVertexCount(struct nGraph *, int);
void setVertexLabel(struct nGraph *, int, char *);

// Metrics and parameters 
int getVertexColor(struct nGraph *, int);
int eccentricity(struct nGraph *, int);
int getEdgeWeight(struct nGraph *, int, int);
int graphRadius(struct nGraph *);
int graphDiameter(struct nGraph *);
struct nGraph graphCenter(struct nGraph *);
void degreeHistogram(struct nGraph *);

// Algorithms
int shortestPathLength(struct nGraph *, int, int);
void BronKerbosch(struct nGraph *, struct nGraph *, struct nGraph *, struct nGraph *);

// Subsets
struct nGraph getVertices(struct nGraph *);
struct nGraph getNeighbours(struct nGraph *, int);
void placeNeighbours(struct nGraph *, int, struct nGraph *);
struct nGraph getCommonNeighbors(struct nGraph *, int, int);
int commonNeighbors(struct nGraph *, int, int);
double jaccardCoefficient(struct nGraph *, int, int);
double adamicAdarIndex(struct nGraph *, int, int);

// Generate
struct nGraph newRandomTree(char *, int);
struct nGraph newCompleteGraph(char *, int);
struct nGraph newPath(char *, int);
struct nGraph newRing(char *, int);
struct nGraph newStar(char *, int);
struct nGraph newErdosRenyiGNP(char *, int, double);
struct nGraph newErdosRenyiGNM(char *, int, int);
struct nGraph newBarabasiAlbert(char *, struct nGraph *, int);
struct nGraph newHyperCube(char *, int);
struct nGraph newPetersenGraph(char *);
struct nGraph newWheel(char *, int);

// Binary operations
struct nGraph gUnion(struct nGraph *, struct nGraph *);
struct nGraph gRingSum(struct nGraph *, struct nGraph *);
struct nGraph gIntersection(struct nGraph *, struct nGraph *);
struct nGraph crossProduct(struct nGraph *, struct nGraph *);
struct nGraph gUnionVertex(struct nGraph *, int);

// Extra
int factorial(int);
int NcR(int, int);
#endif
