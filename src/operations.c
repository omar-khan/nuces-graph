#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nucesGraph.h"

/**
 * Perform a Cross Product of two graphs \f$G_{3} = G_{1} \times G_{2}\f$. The
 * label of the graph \f$G_{3}\f$ reflects label elements of both \f$G_{1}\f$
 * and \f$G_{2}\f$. Likewise, the label of vertices of graph \f$G_{3}\f$
 * reflects adjacency of vertices of \f$G_{1}\f$ with those of \f$G_{2}\f$.
 * @param G1 Graph Object 1
 * @param G2 Graph Object 2
 * @return Graph Object 3
 */
struct nGraph crossProduct(struct nGraph *G1, struct nGraph *G2)
{
	/* Cross the Graph Labels */
	int newLabelSize = strlen(G1->label) + strlen(G2->label) + 2;
	char *newLabel = calloc(newLabelSize, sizeof(char));
	strncat(newLabel, G1->label, strnlen(G1->label, newLabelSize-strlen(newLabel)-1));
	strcat(newLabel, "x");
	strncat(newLabel, G2->label, strnlen(G2->label, newLabelSize-strlen(newLabel)-1));
	strncat(newLabel, "\0", sizeof(char));
	struct nGraph tmp = newGraph(newLabel);
	free(newLabel);

	/* Convert integer vertex labels to string vertex labels */
	struct vertex *utmp = G1->V->head;
	int size;
	while(utmp != NULL) {
		if (utmp->lblString == NULL) {
			size = (int)ceil(log(utmp->label == 0 ? utmp->label + 2 : utmp->label + 1)/log(10))+1;
			utmp->lblString = malloc(sizeof(char)*size);
			snprintf(utmp->lblString, sizeof(char)*size, "%d", utmp->label);
		}
		utmp = utmp->next;
	}

	/* Convert integer vertex labels to string vertex labels */
	struct vertex *vtmp = G2->V->head;
	while(vtmp != NULL) {
		if (vtmp->lblString == NULL) {
			size = (int)ceil(log(vtmp->label == 0 ? vtmp->label + 2 : vtmp->label + 1)/log(10))+1;
			vtmp->lblString = malloc(sizeof(char)*size);
			snprintf(vtmp->lblString, sizeof(char)*size, "%d", vtmp->label);
		}
		vtmp = vtmp->next;
	}

	/* Add Vertices */
	utmp = G1->V->head;
	while (utmp != NULL) {
		vtmp = G2->V->head;
		while (vtmp != NULL) {

			addVertex(&tmp, tmp.V->count);
			size = (int)(strlen(utmp->lblString)+strlen(vtmp->lblString)+strlen(","))+1;
			tmp.V->tail->lblString = calloc(size, sizeof(char));//malloc(sizeof(char)*(size));
			strncat(tmp.V->tail->lblString, utmp->lblString, strnlen(utmp->lblString, size-strlen(tmp.V->tail->lblString)-1));
			strcat(tmp.V->tail->lblString, ",");
			strncat(tmp.V->tail->lblString, vtmp->lblString, strnlen(vtmp->lblString, size-strlen(tmp.V->tail->lblString)-1));
			//strncpy(tmp.V->tail->lblString, utmp->lblString, sizeof(char)*(int)strlen(utmp->lblString));
			//strcat(tmp.V->tail->lblString, ",");
			//strncat(tmp.V->tail->lblString, vtmp->lblString, sizeof(char)*(int)strlen(vtmp->lblString));

			vtmp = vtmp->next;
		}
		utmp = utmp->next;
	}

	if (tmp.V->count > 512) {
		setDisplayType(&tmp, "sfdp");
	}

	/* Add Edges, taking into account adjacency */
	struct edge *edgeTmp = G1->E->head;
	while (edgeTmp != NULL) {
		addEdge(&tmp, edgeTmp->head, edgeTmp->tail, 0);
		addEdge(&tmp, edgeTmp->head + G1->V->count, edgeTmp->tail + G1->V->count, 0);
		addEdge(&tmp, edgeTmp->head, edgeTmp->head + G1->V->count, 0);
		addEdge(&tmp, edgeTmp->tail, edgeTmp->tail + G1->V->count, 0);

		edgeTmp = edgeTmp->next;
	}
	
	return tmp;
}

struct nGraph gUnionVertex(struct nGraph *ONE, int label)
{
	struct nGraph result;
	nGraphInit(&result, "U");

	// get all vertices from ONE
	struct vertex *tmp = ONE->V->head;
	while(tmp != NULL) {
		addVertex(&result, tmp->label); 
		tmp = tmp->next;
	}
	if (!searchVertex(&result, label)) {
		addVertex(&result, label);
	}
	return result;
}

struct nGraph gUnion(struct nGraph *ONE, struct nGraph *TWO)
{
	struct nGraph result;
	nGraphInit(&result, "U");

	// get all vertices from ONE
	struct vertex *tmp = ONE->V->head;
	while(tmp != NULL) {
		addVertex(&result, tmp->label);
		if (tmp->lblString != NULL) {
			setVertexLabel(&result, tmp->label, tmp->lblString);
		}
		tmp = tmp->next;
	}
	// get all vertices from TWO and add if not exists
	tmp = TWO->V->head;
	while(tmp != NULL) {
		if (!searchVertex(&result, tmp->label)) {
			addVertex(&result, tmp->label);
			if (tmp->lblString != NULL) {
				setVertexLabel(&result, tmp->label, tmp->lblString);
			}
		}
		tmp = tmp->next;
	}

	// get all edges from ONE
	struct edge *tmpe = ONE->E->head;
	while (tmpe != NULL) {
		addEdge(&result, tmpe->head, tmpe->tail, tmpe->weight);
		tmpe = tmpe->next;
	}
	// get all edges from two if not exists
	tmpe = TWO->E->head;
	while (tmpe != NULL) {
		if (!edgeExists(&result, tmpe->head, tmpe->tail)) {
			addEdge(&result, tmpe->head, tmpe->tail, tmpe->weight);
		}
		tmpe = tmpe->next;
	}

	return result;
}

struct nGraph gIntersection(struct nGraph *ONE, struct nGraph *TWO)
{
	struct nGraph result;
	nGraphInit(&result, "N");

	struct vertex *tmp = ONE->V->head;
	while(tmp != NULL) {
		if (searchVertex(TWO, tmp->label)) {
			addVertex(&result, tmp->label);
		}
		tmp = tmp->next;		
	}
	tmp = TWO->V->head;
	while(tmp != NULL) {
		if (searchVertex(ONE, tmp->label)) {
			if (!searchVertex(&result, tmp->label)) {
				addVertex(&result, tmp->label);
			}
		}
		tmp = tmp->next;		
	}

	struct edge *tmpE = ONE->E->head;
	while(tmpE != NULL) {
		if (edgeExists(TWO, tmpE->head, tmpE->tail)) {
			addEdge(&result, tmpE->head, tmpE->tail, tmpE->weight);
		}
		tmpE = tmpE->next;
	}
	tmpE = TWO->E->head;
	while(tmpE != NULL) {
		if (edgeExists(ONE, tmpE->head, tmpE->tail)) {
			if (!edgeExists(&result, tmpE->head, tmpE->tail)) {
				addEdge(&result, tmpE->head, tmpE->tail, tmpE->weight);
			}
		}
		tmpE = tmpE->next;
	}

	return result;
}

struct nGraph gRingSum(struct nGraph *ONE, struct nGraph *TWO)
{
	struct nGraph result;
	nGraphInit(&result, "R");

	// get all vertices from ONE
	struct vertex *tmp = ONE->V->head;
	while(tmp != NULL) {
		addVertex(&result, tmp->label); 
		tmp = tmp->next;
	}
	// get all vertices from TWO and add if not exists
	tmp = TWO->V->head;
	while(tmp != NULL) {
		if (!searchVertex(&result, tmp->label)) {
			addVertex(&result, tmp->label);
		}
		tmp = tmp->next;
	}

	struct edge *tmpE = ONE->E->head;
	while(tmpE != NULL) {
		if (!edgeExists(TWO, tmpE->head, tmpE->tail)) {
			addEdge(&result, tmpE->head, tmpE->tail, tmpE->weight);
		}
		tmpE = tmpE->next;
	}
	tmpE = TWO->E->head;
	while(tmpE != NULL) {
		if (!edgeExists(ONE, tmpE->head, tmpE->tail)) {
			if (!edgeExists(&result, tmpE->head, tmpE->tail)) {
				addEdge(&result, tmpE->head, tmpE->tail, tmpE->weight);
			}
		}
		tmpE = tmpE->next;
	}

	return result;
}


