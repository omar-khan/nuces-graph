#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nucesGraph.h"

struct nGraph crossProduct(struct nGraph *G1, struct nGraph *G2)
{
	struct nGraph tmp = newGraph(strcat(strcat(G1->label, "x"), G2->label));

	struct vertex *utmp = G1->V->head;
	while(utmp != NULL) {
		if (utmp->lblString == NULL) {
			utmp->lblString = malloc(sizeof(char)*(int)ceil(log(utmp->label == 0 ? utmp->label+2 : utmp->label+1)/(double)log(10)));
			sprintf(utmp->lblString, "%d", utmp->label);
		}
		utmp = utmp->next;
	}
	struct vertex *vtmp = G2->V->head;
	while(vtmp != NULL) {
		if (vtmp->lblString == NULL) {
			vtmp->lblString = malloc(sizeof(char)*(int)ceil(log(vtmp->label == 0 ? vtmp->label+2 : vtmp->label+1)/(double)log(10)));
			sprintf(vtmp->lblString, "%d", vtmp->label);
		}
		vtmp = vtmp->next;
	}

	utmp = G1->V->head;
	while (utmp != NULL) {
		vtmp = G2->V->head;
		while (vtmp != NULL) {

			addVertex(&tmp, tmp.V->count);
			tmp.V->tail->lblString = malloc(sizeof(char)*((int)(strlen(utmp->lblString)+strlen(vtmp->lblString)+strlen(","))));
			strcpy(tmp.V->tail->lblString, utmp->lblString);
			strcpy(tmp.V->tail->lblString, strcat(tmp.V->tail->lblString, ","));
			strcpy(tmp.V->tail->lblString, strcat(tmp.V->tail->lblString, vtmp->lblString));

			vtmp = vtmp->next;
		}
		utmp = utmp->next;
	}

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


