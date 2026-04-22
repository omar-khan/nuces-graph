#include "nucesGraph.h"
#include <fcntl.h>
#include <glpk.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/** Exports a Graph in DIMAC format to a file. Output is written to G.dimac
 * where G is the graph label. At the moment, only network flow graphs are
 * supported.
 * @param G Graph object
 * @param file Filename
 */
void exportDimac(struct nGraph *G) {
  char *s = malloc(sizeof(char) * (G->E->count * 8 + 64));
  makeDimac(G, s);

  char *filename = malloc(sizeof(char) * 128);
  strcpy(filename, G->label);
  strcat(filename, ".dimac");
  FILE *fd = fopen(filename, "w");
  fprintf(fd, s);
  fclose(fd);
  fprintf(stdout, "Output written to %s\n", filename);
}

void makeDimac(struct nGraph *G, char *contents) {
  char *returnString = malloc(sizeof(char) * 128);

  struct edge *tmp = G->E->head;

  sprintf(returnString, "p max %d %d\n", G->V->count, G->E->count);
  strcat(contents, returnString);

  sprintf(returnString, "n %d s\n", 1);
  strcat(contents, returnString);

  sprintf(returnString, "n %d t\n", G->V->count);
  strcat(contents, returnString);

  while (tmp != NULL && G->E->count > 0) {
    if (tmp->directed == 1) {
      sprintf(returnString, "a %d %d %d\n", tmp->head, tmp->tail, tmp->weight);
    } else {
      sprintf(returnString, "p %d %d\n", tmp->head, tmp->tail);
    }
    strcat(contents, returnString);
    tmp = tmp->next;
  }
}

/** Exports GLPK Compatible Linear Program for solving maxflow problems
 * @param G Graph object
 */
void exportGLPK(struct nGraph *G) {
  // Conversion is not direct. First Dimac output is created. Then that Dimac
  // output is again plugged into the GLPK API to create GLPK compatible LP code

  char *code = malloc(sizeof(char) * (G->E->count * 8 + 64));
  makeDimac(G, code);

  char *filename = malloc(sizeof(char) * 128);
  char *filenamelp = malloc(sizeof(char) * 128);
  strcpy(filename, "/tmp/");
  strcpy(filenamelp, "/tmp/");

  strcat(filename, G->label);
  strcat(filenamelp, G->label);

  strcat(filename, ".dimac");
  strcat(filenamelp, ".lp");

  FILE *fd = fopen(filename, "w");
  fprintf(fd, code);
  fclose(fd);
  fprintf(stdout, "Dimac Output written to %s\n", filename);

  glp_prob *lp;
  int s, t;
  glp_graph *g = glp_create_graph(0, sizeof(double));
  glp_read_maxflow(g, &s, &t, 0, filename);
  lp = glp_create_prob();
  glp_maxflow_lp(lp, g, GLP_ON, s, t, 0);
  glp_delete_graph(g);
  glp_write_lp(lp, NULL, filenamelp);
  glp_delete_prob(lp);

  // unlink(filename);  // for deleting dot file
  free(filename);
  free(filenamelp);
}

/** Shows a Graph in DIMAC format. Can be manually copied pasted from the
 * terminal.
 * @param G Graph object
 */
void showDimac(struct nGraph *G) {
  char *s = malloc(sizeof(char) * 1024);
  makeDimac(G, s);

  printf("%s\n", s);
}

void exportDot(struct nGraph *B) {
  /**
   * Export Dot is this function
   */
  fprintf(stdout, "graph %s {\n", B->label);
  //	printf("\toverlap = false;\n");
  fprintf(stdout, "\tnode [shape=\"circle\"];\n");

  struct vertex *tmpV = B->V->head;
  while (tmpV != NULL) {
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
    } else {
      fprintf(stdout, "\t%d -- %d[label=\"%d\"];\n", tmp->head, tmp->tail,
              tmp->weight);
    }
    tmp = tmp->next;
  }
  fprintf(stdout, "}\n");
}

void showDot(struct nGraph *B) {
  static char template[] = "/tmp/nucesGraphXXXXXX";
  char *filename = malloc(sizeof(char) * PATH_MAX);
  char *fileimage = malloc(sizeof(char) * PATH_MAX);
  strcpy(filename, template);
  if (mkstemp(filename) == -1) {
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
    while (tmpV != NULL) {
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
        } else {
          fprintf(tf, "\t%d -> %d[dir=none,label=\"%d\"];\n", tmp->head,
                  tmp->tail, tmp->weight);
        }
      } else {
        if (tmp->weight == 0) {
          fprintf(tf, "\t%d -> %d;\n", tmp->head, tmp->tail);
        } else {
          fprintf(tf, "\t%d -> %d[label=\"%d\"];\n", tmp->head, tmp->tail,
                  tmp->weight);
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
    switch (B->displayType) {
		case 3: 
      execl("/usr/bin/dot", "/usr/bin/dot", filename, "-T", "pdf", "-o",
            fileimage, NULL);
      break;
    case 2:
      execl("/usr/bin/twopi", "/usr/bin/twopi", filename, "-T", "pdf", "-o",
            fileimage, NULL);
      break;
    case 1:
      execl("/usr/bin/sfdp", "/usr/bin/sfdp", filename, "-T", "pdf", "-o",
            fileimage, NULL);
    default:
      execl("/usr/bin/neato", "/usr/bin/neato", filename, "-T", "pdf", "-o",
            fileimage, NULL);
      break;
    }
    printf("i should not be printed\n");
  } else if (pid > 0) {
    wait(NULL);
  }

  // launch window (okular)
  if (getenv("DISPLAY")) {
    pid = fork();
    if (pid == 0) {
      execl("/usr/bin/okular", "/usr/bin/okular", fileimage, NULL);
      printf("i should not be printed\n");
    } else if (pid > 0) {
      // wait(NULL);      // for pausing main while pdf is displayed
    }
  }
  // unlink(filename);  // for deleting dot file
  // unlink(fileimage); // for deleting pdf file
  free(filename);
  free(fileimage);
}

void show(struct nGraph *B) {
  listVertices(B);
  listEdges(B);
}

void listVerticesAlphabet(struct nGraph *G) {
  struct vertex *tmp = G->V->head;
  printf("%s.V = { ", G->label);
  int count = 0;
  while (tmp != NULL) {
    if (tmp->lblString != NULL) {
      printf("%s", tmp->lblString);
    } else {
      printf("%c", 'a' + tmp->label);
    }
    printf("%s", G->V->count - 1 == count ? " " : ", ");
    tmp = tmp->next;
    count++;
  }
  printf("}\n");
}

void listBK_temp(struct nGraph *G) {
  struct vertex *tmp = G->V->head;
  printf("%s%s = { %s", KRED, G->label, KBLU);
  int count = 0;
  while (tmp != NULL && G->V->count > 0) {
    if (tmp->lblString != NULL && strlen(tmp->lblString) > 0) {
      printf("(%d) %s", tmp->label, tmp->lblString);
    } else {
      printf("%d", tmp->label);
    }
    printf("%s", G->V->count - 1 == count ? " " : ", ");
    tmp = tmp->next;
    count++;
  }
  printf("%s}%s", KRED, KWHT);
}

void listVertices(struct nGraph *G) {
  struct vertex *tmp = G->V->head;
  printf("%s%s.V(%d) = { ", KRED, G->label, G->V->count);
  int count = 0;
  while (tmp != NULL && G->V->count > 0) {
    if (tmp->lblString != NULL && strlen(tmp->lblString) > 0) {
      printf("(%d) %s", tmp->label, tmp->lblString);
    } else {
      printf("%d", tmp->label);
    }
    printf("%s", G->V->count - 1 == count ? " " : ", ");
    tmp = tmp->next;
    count++;
  }
  printf("}\n%s", KWHT);
}

/**
 * Displays a List of Edges in a Graph. Output shows the Graph Label, followed
 * by a list having format as: (Edge Id) vertex from, vertex to.
 * @param Graph Object
 */

void listEdges(struct nGraph *G) {
  struct edge *tmp = G->E->head;
  printf("%s%s.E(%d) = { ", KRED, G->label, G->E->count);
  int count = 0;
  while (tmp != NULL && G->E->count > 0) {
    if (count == (G->E->count - 1)) {
      printf("(%d) %d->%d ", tmp->label, tmp->head, tmp->tail);
    } else {
      printf("(%d) %d->%d, ", tmp->label, tmp->head, tmp->tail);
    }
    tmp = tmp->next;
    count++;
  }
  printf("}\n%s", KWHT);
}

void setDisplayType(struct nGraph *G, char *type) {
  if (strcmp(type, "circular") == 0) {
    G->displayType = 2;
  } else if (strcmp(type, "sfdp") == 0) {
    G->displayType = 1;
  } else if (strcmp(type, "dot") == 0) {
		G->displayType = 3;
	} else {
    G->displayType = 0;
  }
}

/**
 * Exports a Graph to TikZ format for use in LaTeX papers.
 * Outputs to a file named after the graph's label (e.g., G.tex).
 * Requires \\usetikzlibrary{graphs,graphdrawing} and LuaLaTeX to render.
 * @param Graph Object
 */
void exportTikZ(struct nGraph *G) {
  char filename[128];
  sprintf(filename, "%s.tex", G->label);

  FILE *fd = fopen(filename, "w");
  if (!fd) {
    fprintf(stderr, "Error: Could not open %s for writing\n", filename);
    return;
  }

  fprintf(fd, "%% Build using LuaLaTeX\n");
	fprintf(fd, "\\documentclass[]{article}\n");
	fprintf(fd, "\\usepackage{tikz}\n");
  fprintf(fd, "\\usetikzlibrary{graphs, graphdrawing}\n");
	fprintf(fd, "\\usegdlibrary{force, layered, trees}\n");
	fprintf(fd, "\\begin{document}\n");

  fprintf(fd, "\\begin{tikzpicture}[>=stealth, every node/.style={circle, draw, minimum size=0.5cm}]\n");
  fprintf(fd, "\\graph [spring layout] {\n");

  // Process Nodes First
	struct vertex *tmpV = G->V->head;
  while (tmpV != NULL) {
    fprintf(fd, "  %d", tmpV->label);
    if (tmpV->lblString != NULL && strlen(tmpV->lblString) > 0) {
      fprintf(fd, " [as=\"%s\"]", tmpV->lblString);
    }
    fprintf(fd, ";\n");
    tmpV = tmpV->next;
  }
  fprintf(fd, "\n");

  // Edges Now
  struct edge *tmpE = G->E->head;
  while (tmpE != NULL) {
    if (tmpE->directed) {
      fprintf(fd, "  %d -> %d", tmpE->head, tmpE->tail);
    } else {
      fprintf(fd, "  %d -- %d", tmpE->head, tmpE->tail);
    }

    if (tmpE->weight != 0) {
      fprintf(fd, " [edge label=%d]", tmpE->weight);
    }
    fprintf(fd, ";\n");

    tmpE = tmpE->next;
  }

  fprintf(fd, "};\n");
  fprintf(fd, "\\end{tikzpicture}\n");
	fprintf(fd, "\\end{document}\n");
  fclose(fd);
  fprintf(stdout, "TikZ output written to %s\\n", filename);
}

/**
 * Exports a Graph to GraphML format for loading into Gephi or Cytoscape.
 * Outputs to a file named after the graph's label (e.g., G.graphml).
 * @param Graph Object
 */
void exportGraphML(struct nGraph *G) {
  char filename[128];
  sprintf(filename, "%s.graphml", G->label);

  FILE *fd = fopen(filename, "w");
  if (!fd) {
    fprintf(stderr, "Error: Could not open %s for writing\\n", filename);
    return;
  }

  fprintf(fd, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(fd, "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\">\n");
  fprintf(fd, "  <key id=\"d0\" for=\"node\" attr.name=\"label\" "
              "attr.type=\"string\" />\n");
  fprintf(fd, "  <key id=\"d1\" for=\"edge\" attr.name=\"weight\" "
              "attr.type=\"int\" />\n");

  // Graph element
  if (G->directed) {
    fprintf(fd, "  <graph id=\"%s\" edgedefault=\"directed\">\n", G->label);
  } else {
    fprintf(fd, "  <graph id=\"%s\" edgedefault=\"undirected\">\n", G->label);
  }

  // Nodes
  struct vertex *tmpV = G->V->head;
  while (tmpV != NULL) {
    fprintf(fd, "    <node id=\"n%d\">\n", tmpV->label);

    char *labelStr = (tmpV->lblString != NULL) ? tmpV->lblString : "";
    if (strlen(labelStr) > 0) {
      fprintf(fd, "      <data key=\"d0\">%s</data>\n", labelStr);
    } else {
      fprintf(fd, "      <data key=\"d0\">%d</data>\n", tmpV->label);
    }

    fprintf(fd, "    </node>\n");
    tmpV = tmpV->next;
  }

  // Edges
  struct edge *tmpE = G->E->head;
  int edge_id = 0;
  while (tmpE != NULL) {
    fprintf(fd, "    <edge id=\"e%d\" source=\"n%d\" target=\"n%d\"", edge_id++,
            tmpE->head, tmpE->tail);

    // If specific edge directedness differs from graph default
    if (tmpE->directed != G->directed) {
      fprintf(fd, " directed=\"%s\"", tmpE->directed ? "true" : "false");
    }

    if (tmpE->weight != 0) {
      fprintf(fd, ">\n      <data key=\"d1\">%d</data>\n    </edge>\n",
              tmpE->weight);
    } else {
      fprintf(fd, " />\n");
    }

    tmpE = tmpE->next;
  }

  fprintf(fd, "  </graph>\n");
  fprintf(fd, "</graphml>\n");

  fclose(fd);
  fprintf(stdout, "GraphML output written to %s\n", filename);
}
