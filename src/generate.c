/**
 * @brief This file contains code snippets for generating various families of
 * graphs.
 */

#include "nucesGraph.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>

/**
 * Constructs a random binary tree
 * @param C Label for the tree
 * @param V Number of vertices required in the tree
 * @return Graph object
 */

struct nGraph newRandomBinaryTree(char *C, int V) {
  struct nGraph tmp = newGraph(C);
  struct nGraph NotDone = newGraph("N");

  int *childrenCount = calloc(V, sizeof(int));
  int *doneNodes = malloc(V * sizeof(int));
  int doneCount = 0;

  for (int i = 0; i < V; i++) {
    addVertex(&NotDone, i);
  }
  srand(time(0));

  int root = rand() % V; // Random Root
  addVertex(&tmp, root);
  setVertexLabel(&tmp, root, "R");
  removeVertex(&NotDone, root);

  doneNodes[doneCount++] = root;

  while (doneCount < V) {
    int v;
    do {
      v = rand() % V; // Random Vertice not in Tree Yet
    } while (searchVertex(&tmp, v));

    int parentIdx;
    int parent;
    do { // Random parent from tree with < 2 children
      parentIdx = rand() % doneCount;
      parent = doneNodes[parentIdx];
    } while (childrenCount[parent] >= 2);

    addVertex(&tmp, v);
    addEdge(&tmp, parent, v, 0);

    childrenCount[parent]++;
    doneNodes[doneCount++] = v;
  }

  free(childrenCount);
  free(doneNodes);

  setDisplayType(&tmp, "dot");
  return tmp;
}

/**
 * Constructs a random tree.
 * @param C Label for the tree
 * @param V Number of vertices required in the tree
 * @return Graph object
 */

struct nGraph newRandomTree(char *C, int V) {
  struct nGraph tmp = newGraph(C);

  struct nGraph Done = newGraph("D");
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

void makeGraphComplete(struct nGraph *G) {
  struct vertex *v1 = G->V->head;
  while (v1 != NULL) {
    struct vertex *v2 = v1->next;
    while (v2 != NULL) {
      if (!edgeExists(G, v1->label, v2->label)) {
        addEdge(G, v1->label, v2->label, 1);
      }
      v2 = v2->next;
    }
    v1 = v1->next;
  }
}

/**
 * Constructs a Complete Graph \f$K_{n}\f$ of \f$n\f$ vertices, where degree of
 * each vertex is \f$n-1\f$.
 * @param c Graph Label
 * @param n Number of Vertices
 * @return Graph Object
 */

struct nGraph newCompleteGraph(char *c, int n) {
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

struct nGraph newStar(char *c, int n) {
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

struct nGraph newPath(char *c, int n) {
  struct nGraph tmp = newGraph(c);

  int i;
  for (i = 0; i < n; i++) {
    addVertex(&tmp, i);
  }

  for (i = 0; i < n - 1; i++) {
    addEdge(&tmp, i, i + 1, 1);
  }

  return tmp;
}

/**
 * Constructs a Ring graph \f$P_{n}\f$ of n vertices
 * @param c Label for the Graph
 * @param n Number of Vertices
 * @return Graph object
 */

struct nGraph newRing(char *c, int n) {
  struct nGraph tmp = newPath(c, n);
  addEdge(&tmp, 0, tmp.V->count - 1, 1);
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
struct nGraph newErdosRenyiGNP(char *lbl, int N, double P) {
  struct nGraph tmp = newGraph(lbl);

  int i, j;
  for (i = 0; i < N; i++) {
    addVertex(&tmp, i);
  }

  double rnd;
  srand(clock());
  for (j = 0; j < N; j++) {
    for (i = j + 1; i < N; i++) {
      if (i != j) {
        rnd = rand() / (double)RAND_MAX;
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
struct nGraph newErdosRenyiGNM(char *lbl, int N, int M) {
  struct nGraph tmp = newGraph(lbl);

  int i, j;
  for (i = 0; i < N; i++) {
    addVertex(&tmp, i);
  }

  double P = M / (double)((N * N - 1) / 2.);
  int count = 0;

  double rnd;
  srand(clock());

  while (count < M) {
    i = rand() % N;
    j = rand() % N;
    while (i == j) {
      j = rand() % N;
    }
    rnd = rand() / (double)RAND_MAX;
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

struct nGraph newBarabasiAlbert(char *lbl, struct nGraph *pattern, int M) {
  struct nGraph tmp = newGraph(lbl);
  copyGraph(&tmp, pattern);

  int i, count = 0;
  double P, rnd;
  struct vertex *tmp_vertex;

  i = 0;
  while (i < M) {
    tmp_vertex = tmp.V->head;
    count = 0;
    while (count < pattern->V->count && tmp_vertex != NULL) {
      P = tmp_vertex->degree_in / (double)(2 * tmp.E->count);
      rnd = rand() / (double)RAND_MAX;
      if (rnd < P) {
        addVertex(&tmp, tmp.V->count);
        addEdge(&tmp, tmp.V->count - 1, tmp_vertex->label, 1);
        count++;
        i++;
      }
      tmp_vertex = tmp_vertex->next;
    }
  }
  return tmp;
}

/**
 * Returns a hypercube of a given order. Note: Viewing hypercubes of order 10
 * and above using showDot() takes too much time.
 * @param lbl Label for the Hypercube graph
 * @param order Order of the Hypercube graph
 * @return Graph object
 */

struct nGraph newHyperCube(char *lbl, int order) {
  struct nGraph tmp[order];
  tmp[0] = newPath(lbl, 2);
  struct nGraph P2 = newPath(lbl, 2);

  int i;
  for (i = 1; i < order; i++) {
    tmp[i] = crossProduct(&tmp[i - 1], &P2);
    nGraphFree(&tmp[i - 1]);
  }

  nGraphFree(&P2);
  return tmp[i - 1];
}

/**
 * Returns Petersen's graph of 10 vertices and 15 edges
 * @param lbl Label for the graph
 * @return Graph object
 */

struct nGraph newPetersenGraph(char *lbl) {
  struct nGraph tmp = newGraph(lbl);
  int i;
  for (i = 0; i < 5; i++) {
    addVertex(&tmp, i);
    addVertex(&tmp, i + 5);
  }
  for (i = 0; i < 10; i += 2) {
    addEdge(&tmp, i % 5, (i + 2) % 5, 1); // Inner Star
    addEdge(&tmp, i / 2 + 5, (i / 2 + 6) % 5 + 5, 1);
    addEdge(&tmp, i / 2, (i / 2) + 5, 1);
  }
  return tmp;
}

/* Returns a Wheel Graph of \f$n \geq 4\f$ vertices.
 * @param lbl Label for the graph
 * @param n Number of vertices
 * @return Graph object
 */

struct nGraph newWheel(char *lbl, int n) {
  struct nGraph tmp = newGraph(lbl);

  int i;
  for (i = 0; i < n; i++) {
    addVertex(&tmp, i);
  }

  for (i = 1; i < n; i++) {
    addEdge(&tmp, i, i == n - 1 ? 1 : i + 1, 1);
    addEdge(&tmp, 0, i, 1);
  }

  return tmp;
}

static int isIntegerString(const char *str) {
  if (!str || *str == '\0')
    return 0;
  int i = 0;
  if (str[0] == '-')
    i++;
  for (; str[i] != '\0'; i++) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
  }
  return 1;
}

static void clean_str(char *src, char *dst) {
  char *s = src;
  while (*s == ' ' || *s == '\t')
    s++;
  int len = strlen(s);
  while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' ||
                     s[len - 1] == '\n' || s[len - 1] == '\r'))
    len--;
  s[len] = '\0';
  if (s[0] == '"' && s[len - 1] == '"' && len >= 2) {
    s[len - 1] = '\0';
    strcpy(dst, s + 1);
  } else {
    strcpy(dst, s);
  }
}

/**
 * Imports a graph from a Graphviz dot format file.
 * Automatically handles string labels vs integer IDs and supports
 * directed/undirected edges.
 * @param lbl Label for the new graph
 * @param filename Path to the dot file
 * @return Graph object
 */
struct nGraph importDot(char *lbl, char *filename) {
  struct nGraph G = newGraph(lbl);
  FILE *fd = fopen(filename, "r");
  if (!fd) {
    fprintf(stderr, "Error: Could not open %s for reading\n", filename);
    return G;
  }

  char line[1024];
  int firstLine = 1;

  while (fgets(line, sizeof(line), fd)) {
    if (firstLine) {
      if (strstr(line, "digraph")) {
        G.directed = 1;
      }
      // setDisplayType(&G, "dot");
      firstLine = 0;
      continue;
    }

    if (strstr(line, "overlap") || strstr(line, "splines") ||
        strstr(line, "sep") || strstr(line, "node [shape=") ||
        strstr(line, "}") || strstr(line, "{") || strstr(line, "graph [") ||
        strstr(line, "edge [") || strstr(line, "node [")) {
      continue;
    }

    char head_str[256] = {0};
    char tail_str[256] = {0};
    char label_str[256] = {0};
    int weight = 0;

    char *edgePtr = strstr(line, "->");
    if (!edgePtr)
      edgePtr = strstr(line, "--");

    if (edgePtr) {
      if (strstr(line, "[label=\"")) {
        char *lblStart = strstr(line, "[label=\"") + 8;
        weight = atoi(lblStart);
      } else if (strstr(line, "[dir=none,label=\"")) {
        char *lblStart = strstr(line, "[dir=none,label=\"") + 17;
        weight = atoi(lblStart);
      }

      int headLen = edgePtr - line;
      strncpy(head_str, line, headLen);
      head_str[headLen] = '\0';

      char *tailStart = edgePtr + 2;
      char *endPtr = strstr(tailStart, "[");
      if (!endPtr)
        endPtr = strstr(tailStart, ";");
      if (!endPtr)
        endPtr = strstr(tailStart, "\n");
      if (endPtr) {
        int tailLen = endPtr - tailStart;
        strncpy(tail_str, tailStart, tailLen);
        tail_str[tailLen] = '\0';
      } else {
        strcpy(tail_str, tailStart);
      }

      char clean_head[256] = {0}, clean_tail[256] = {0};
      clean_str(head_str, clean_head);
      clean_str(tail_str, clean_tail);

      int is_directed =
          (strstr(line, "->") != NULL && strstr(line, "dir=none") == NULL);

      if (isIntegerString(clean_head) && isIntegerString(clean_tail)) {
        int id1 = atoi(clean_head);
        int id2 = atoi(clean_tail);
        addVertex(&G, id1);
        addVertex(&G, id2);
        if (is_directed) {
          addEdgeDirected(&G, id1, id2, weight);
        } else {
          addEdge(&G, id1, id2, weight);
        }
      } else {
        addVertexLabel(&G, clean_head);
        addVertexLabel(&G, clean_tail);
        if (is_directed) {
          addEdgeDirectedLabel(&G, clean_head, clean_tail, weight);
        } else {
          addEdgeLabel(&G, clean_head, clean_tail, weight);
        }
      }

    } else if (strstr(line, "[")) {
      char *lblStart = strstr(line, "label=\"");
      if (lblStart) {
        lblStart += 7;
        int i = 0;
        while (lblStart[i] != '"' && lblStart[i] != '\0' && i < 255) {
          label_str[i] = lblStart[i];
          i++;
        }
        label_str[i] = '\0';
      }

      char *endPtr = strstr(line, "[");
      int headLen = endPtr - line;
      strncpy(head_str, line, headLen);
      head_str[headLen] = '\0';

      char clean_head[256] = {0};
      clean_str(head_str, clean_head);

      if (isIntegerString(clean_head)) {
        int id = atoi(clean_head);
        addVertex(&G, id);
        if (strlen(label_str) > 0) {
          setVertexLabel(&G, id, label_str);
        }
      } else {
        addVertexLabel(&G, clean_head);
      }

    } else if (strspn(line, " \t\n") != strlen(line)) {
      char *endPtr = strstr(line, ";");
      if (!endPtr)
        endPtr = strstr(line, "\n");
      if (endPtr) {
        int headLen = endPtr - line;
        strncpy(head_str, line, headLen);
        head_str[headLen] = '\0';
      } else {
        strcpy(head_str, line);
      }

      char clean_head[256] = {0};
      clean_str(head_str, clean_head);

      if (strlen(clean_head) > 0) {
        if (isIntegerString(clean_head)) {
          addVertex(&G, atoi(clean_head));
        } else {
          addVertexLabel(&G, clean_head);
        }
      }
    }
  }

  fclose(fd);
  return G;
}
