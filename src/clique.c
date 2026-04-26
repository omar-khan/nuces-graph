#include "nucesGraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void BronKerboschMax_Recursive(struct nGraph *G, struct nGraph *P,
                                      struct nGraph *R, struct nGraph *X,
                                      int *max_size,
                                      struct nGraphList **max_cliques) {
  struct nGraph PunionX = gUnion(P, X);
  if (PunionX.V->count == 0) {
    if (R->V->count > *max_size) { // Larger Clique Found. Clear Previous
      nGraphListFree(*max_cliques);
      *max_cliques = NULL;
      *max_size = R->V->count;

      struct nGraphList *newNode =
          (struct nGraphList *)malloc(sizeof(struct nGraphList));
      nGraphInit(&newNode->G, "MAX");
      copyGraph(&newNode->G, R);

      makeGraphComplete(&newNode->G);

      newNode->next = NULL;
      *max_cliques = newNode;
    } else if (R->V->count == *max_size &&
               *max_size > 0) { // Same Clique Size found
      struct nGraphList *newNode =
          (struct nGraphList *)malloc(sizeof(struct nGraphList));
      nGraphInit(&newNode->G, "MAX");
      copyGraph(&newNode->G, R);

      makeGraphComplete(&newNode->G);

      newNode->next = *max_cliques;
      *max_cliques = newNode;
    }
    nGraphFree(&PunionX);
    return;
  }
  nGraphFree(&PunionX);

  struct nGraph P_copy = newGraph("P_copy");
  copyGraph(&P_copy, P);

  struct vertex *tmp = P_copy.V->head;
  while (tmp != NULL) {
    int v = tmp->label;
    struct nGraph neigh = getNeighbours(G, v);

    struct nGraph P_new = gIntersection(P, &neigh);
    struct nGraph R_new = gUnionVertex(R, v);
    // Ensure the label for v is copied to R_new
    char *vLabel = getVertexLabelString(G, v);
    if (vLabel && strcmp(vLabel, "not found") != 0) {
      setVertexLabel(&R_new, v, vLabel);
    }

    struct nGraph X_new = gIntersection(X, &neigh);

    BronKerboschMax_Recursive(G, &P_new, &R_new, &X_new, max_size, max_cliques);

    nGraphFree(&P_new);
    nGraphFree(&R_new);
    nGraphFree(&X_new);
    nGraphFree(&neigh);

    addVertex(X, v);
    if (vLabel && strcmp(vLabel, "not found") != 0) {
      setVertexLabel(X, v, vLabel);
    }
    removeVertex(P, v);

    tmp = tmp->next;
  }
  nGraphFree(&P_copy);
}

struct nGraphList *BronKerboschMax(struct nGraph *G) {
  struct nGraph P = getVertices(G);
  struct nGraph R = newGraph("R");
  struct nGraph X = newGraph("X");

  int max_size = 0;
  struct nGraphList *max_cliques = NULL;

  BronKerboschMax_Recursive(G, &P, &R, &X, &max_size, &max_cliques);

  nGraphFree(&P);
  nGraphFree(&R);
  nGraphFree(&X);

  struct nGraph allCliques = newGraph("All");

  printf("\n--- Maximum Cliques (Size %d) ---\n", max_size);
  struct nGraphList *curr = max_cliques;
  int count = 1;
  while (curr) {
    printf("Clique %d: ", count++);
    listBK_temp(&curr->G);
    printf("\n");
    showDot(&curr->G);

    struct nGraph temp = gUnion(&allCliques, &curr->G);
    nGraphFree(&allCliques);
    allCliques = temp;

    curr = curr->next;
  }

  graphChangeLabel(&allCliques, "All");
  listBK_temp(&allCliques);
  printf("\n");
  showDot(&allCliques);
  nGraphFree(&allCliques);

  return max_cliques;
}

/**
 * This is the Classical Bron Kerbosch method for finding maximum / maximal
 * cliques in a graph. All cliques are displayed as output and are not returned
 * as objects to other parts of the algorithm.
 * @param G Graph object containing original graph
 * @param P set containing vertices of G, represents possible  candidates
 * @param R an empty set representing temporary results
 * @param X an empty set representing excluded set
 */

void BronKerbosch(struct nGraph *G, struct nGraph *P, struct nGraph *R,
                  struct nGraph *X) {
  struct nGraph PunionX = gUnion(P, X);
  if (PunionX.V->count == 0) {
    printf("Maximal: ");
    listBK_temp(R);
    printf("\n");
    nGraphFree(&PunionX);
    return;
  }
  nGraphFree(&PunionX);

  struct nGraph P_copy = newGraph("P_copy");
  copyGraph(&P_copy, P);

  struct vertex *tmp = P_copy.V->head;
  while (tmp != NULL) {
    int v = tmp->label;
    struct nGraph neigh = getNeighbours(G, v);

    struct nGraph P_new = gIntersection(P, &neigh);
    struct nGraph R_new = gUnionVertex(R, v);
    char *vLabel = getVertexLabelString(G, v);
    if (vLabel && strcmp(vLabel, "not found") != 0) {
      setVertexLabel(&R_new, v, vLabel);
    }
    struct nGraph X_new = gIntersection(X, &neigh);

    BronKerbosch(G, &P_new, &R_new, &X_new);

    nGraphFree(&P_new);
    nGraphFree(&R_new);
    nGraphFree(&X_new);
    nGraphFree(&neigh);

    addVertex(X, v);
    if (vLabel && strcmp(vLabel, "not found") != 0) {
      setVertexLabel(X, v, vLabel);
    }
    removeVertex(P, v);

    tmp = tmp->next;
  }
  nGraphFree(&P_copy);
}
