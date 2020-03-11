#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>
#include "nucesGraph.h"

/** Exports a Graph in DIMAC format to a file. Output is written to G.dimac
 * where G is the graph label.
 * @param G Graph object
 * @param file Filename
 */
void exportDimac(struct nGraph *G)
{
	char *s = malloc(sizeof(char)*1024);
	makeDimac(G, s);

	char *filename = malloc(sizeof(char)*128);
	strcpy(filename, G->label);
	strcat(filename, ".dimac");
	FILE *fd = fopen(filename, "w");
	fprintf(fd, s);
	fclose(fd);
	fprintf(stdout, "Output written to %s\n", filename);
}

void makeDimac(struct nGraph *G, char *contents)
{
	char *returnString = malloc(sizeof(char)*128);

	struct edge   *tmp = G->E->head;

	sprintf(returnString, "p edge %d %d\n", G->V->count, G->E->count);
	strcat(contents, returnString);

	while (tmp != NULL && G->E->count > 0) {
		sprintf(returnString, "e %d %d\n", tmp->head, tmp->tail);
		strcat(contents, returnString);
		tmp = tmp->next;
	}
}

/** Shows a Graph in DIMAC format. Can be manually copied pasted from the
 * terminal.
 * @param G Graph object
 */
void showDimac(struct nGraph *G)
{
	char *s = malloc(sizeof(char)*1024);
	makeDimac(G, s);

	printf("%s\n", s);
}

void exportDot(struct nGraph *B)
{
	/** 
	 * Export Dot is this function
	 */
	fprintf(stdout, "graph %s {\n", B->label);
//	printf("\toverlap = false;\n");
	fprintf(stdout, "\tnode [shape=\"circle\"];\n");

	struct vertex *tmpV = B->V->head;
	while(tmpV != NULL) {
		fprintf(stdout, "\t%d", tmpV->label);
		if (tmpV->lblString != NULL) {
			fprintf(stdout, " [label=\"%s\"]", tmpV->lblString);
		}
		fprintf(stdout, ";\n");
		tmpV = tmpV->next;
	}

	struct edge *tmp = B->E->head;
	while (tmp != NULL) {
		if (tmp->weight == 0) {
			fprintf(stdout, "\t%d -- %d;\n", tmp->head, tmp->tail);
		}
		else {
			fprintf(stdout, "\t%d -- %d[label=\"%d\"];\n", tmp->head, tmp->tail, tmp->weight);
		}
		tmp = tmp->next;
	}
	fprintf(stdout, "}\n");
}

void showDot(struct nGraph *B)
{
	static char template[] = "/tmp/nucesGraphXXXXXX";
	char *filename = malloc(sizeof(char)*PATH_MAX);
	char *fileimage = malloc(sizeof(char)*PATH_MAX);
	strcpy(filename, template);
	if (mkstemp(filename)==-1) {
		fprintf(stderr, "showDot(): Temp File Creation Error\n");
	}
	strcpy(fileimage, filename);
	strcat(fileimage, ".pdf");

	FILE *tf = fopen(filename, "w+");
	if (tf) {
		fprintf(tf, "digraph %s {\n", B->label);
		fprintf(tf, "\toverlap = false;\n");
		fprintf(tf, "\tsplines = \"curved\";\n");
		fprintf(tf, "\tsep = 3;\n");
		fprintf(tf, "\tnode [shape=\"circle\"];\n");
		
		struct vertex *tmpV = B->V->head;
		while(tmpV != NULL) {
			fprintf(tf, "\t%d", tmpV->label);
			if (tmpV->lblString != NULL || tmpV->color != -1) {
				fprintf(tf, " [");
				if (tmpV->lblString != NULL) {
					fprintf(tf, "label=\"%s\"", tmpV->lblString);
				}
				if (tmpV->lblString != NULL && tmpV->color != -1) {
					fprintf(tf, ", ");
				}
				if (tmpV->color != -1) {
					fprintf(tf, "style=filled, color=\"%s\"", colors[tmpV->color]);
				}
				fprintf(tf, "]");
			}
			fprintf(tf, ";\n");
			tmpV = tmpV->next;
		}

		struct edge *tmp = B->E->head;
		while (tmp != NULL) {
			if (tmp->directed == 0) {
				if (tmp->weight == 0) {
					fprintf(tf, "\t%d -> %d[dir=none];\n", tmp->head, tmp->tail);
				}
				else {
					fprintf(tf, "\t%d -> %d[dir=none,label=\"%d\"];\n", tmp->head, tmp->tail, tmp->weight);
				}
			} else {
				if (tmp->weight == 0) {
					fprintf(tf, "\t%d -> %d;\n", tmp->head, tmp->tail);
				}
				else {
					fprintf(tf, "\t%d -> %d[label=\"%d\"];\n", tmp->head, tmp->tail, tmp->weight);
				}

			}
			tmp = tmp->next;
		}
		fprintf(tf, "}\n");
	} 
	fclose(tf);

	// convert to dot
	pid_t pid = fork();
	if (pid == 0) {
		printf("Source (dot) at: %s\nImage  (pdf) at: %s\n\n", filename, fileimage);
		switch(B->displayType)
		{
			case 1: execl("/usr/bin/twopi", "/usr/bin/twopi", filename, "-T", "pdf", "-o", fileimage, NULL);
							break;
			default: execl("/usr/bin/neato", "/usr/bin/neato", filename, "-T", "pdf", "-o", fileimage, NULL);
							 break;
		}
		printf("i should not be printed\n");
	}
	else if (pid > 0) {
		wait(NULL);	
	}

	// launch window (okular)
	pid = fork();
	if (pid == 0) {
		execl("/usr/bin/okular", "/usr/bin/okular", fileimage, NULL);
		printf("i should not be printed\n");
	}
	else if (pid > 0) {
		//wait(NULL);
	}
	//unlink(filename);
	//unlink(fileimage);
}

void show(struct nGraph *B)
{
	listVertices(B);
	listEdges(B);
}

void listVerticesAlphabet(struct nGraph *G)
{
	struct vertex *tmp = G->V->head;
	printf("%s.V = { ", G->label);
	int count = 0; 
	while(tmp != NULL) {
		if (tmp->lblString != NULL) {
			printf("%s", tmp->lblString);
		}
		else {
			printf("%c", 'a'+tmp->label);
		}
		printf("%s", G->V->count-1 == count ? " " : ", ");
		tmp = tmp->next;
		count++;
	}
	printf("}\n");
}

void listBK_temp(struct nGraph *G)
{
	struct vertex *tmp = G->V->head;
	printf("%s%s = { %s", KRED, G->label, KBLU);
	int count = 0; 
	while(tmp != NULL && G->V->count > 0) {
		if (tmp->lblString != NULL && strlen(tmp->lblString) > 0) {
			printf("(%d) %s", tmp->label, tmp->lblString);
		}
		else {
			printf("%d", tmp->label);
		}
		printf("%s", G->V->count-1 == count ? " " : ", ");
		tmp = tmp->next;
		count++;
	}
	printf("%s}%s", KRED, KWHT);
}

void listVertices(struct nGraph *G)
{
	struct vertex *tmp = G->V->head;
	printf("%s%s.V(%d) = { ", KRED, G->label, G->V->count);
	int count = 0; 
	while(tmp != NULL && G->V->count > 0) {
		if (tmp->lblString != NULL && strlen(tmp->lblString) > 0) {
			printf("(%d) %s", tmp->label, tmp->lblString);
		}
		else {
			printf("%d", tmp->label);
		}
		printf("%s", G->V->count-1 == count ? " " : ", ");
		tmp = tmp->next;
		count++;
	}
	printf("}\n%s", KWHT);
}

void listEdges(struct nGraph *G)
{
	struct edge *tmp = G->E->head;
	printf("%s%s.E(%d) = { ", KRED, G->label, G->E->count);
	int count = 0; 
	while (tmp != NULL && G->E->count > 0) {
		if (count == (G->E->count-1)) {
			printf("%d->%d ", tmp->head, tmp->tail);
		}
		else {
			printf("%d->%d, ", tmp->head, tmp->tail);
		}
		tmp = tmp->next;
		count++;
	}
	printf("}\n%s\n", KWHT);
}

void setDisplayType(struct nGraph *G, char *type)
{
	if (strcmp(type, "circular") == 0) {
		G->displayType = 1;
	} else {
		G->displayType = 0;
	}

}
