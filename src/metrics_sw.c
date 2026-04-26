#include "nucesGraph.h"
#include <clang-c/Index.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



struct CursorList {
  CXCursor cursors[128];
  int count;
};

/**
 * DJB2 Hash Algorithm by Dan Bernstein. Code iterates through string $str$
 * character by character until it hits null character. With each character, a
 * hash is calculated as $H = 2^{5} H + H + c$ where $c$ is the ASCII integer
 * value of the character.
 * @param str Input String
 * @return hash value
 */
unsigned long hash_string(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

void clearFlowGraphs(struct CallGraphBuilder *b)
{
	nGraphFree(&b->CFG);
	nGraphFree(&b->DFG);
}

int is_significant(enum CXCursorKind kind) {
  return (kind == CXCursor_CompoundStmt || kind == CXCursor_IfStmt ||
          kind == CXCursor_ForStmt || kind == CXCursor_WhileStmt ||
          kind == CXCursor_DoStmt || kind == CXCursor_CallExpr ||
          kind == CXCursor_ReturnStmt || kind == CXCursor_DeclStmt ||
          kind == CXCursor_BinaryOperator || kind == CXCursor_UnaryOperator);
}

enum CXChildVisitResult collect_children_visitor(CXCursor cursor,
                                                 CXCursor parent,
                                                 CXClientData client_data) {
  struct CursorList *list = (struct CursorList *)client_data;
  if (is_significant(clang_getCursorKind(cursor)) && list->count < 128) {
    list->cursors[list->count++] = cursor;
  }
  return CXChildVisit_Continue;
}

void link_tails(struct CallGraphBuilder *builder, int target_id) {
  if (builder->tail_count == 0 && builder->current_func_entry != 0) {
    addEdgeDirected(&builder->CFG, builder->current_func_entry, target_id, 1);
  }
  for (int i = 0; i < builder->tail_count; i++) {
    addEdgeDirected(&builder->CFG, builder->tails[i], target_id, 1);
  }
  builder->tails[0] = target_id;
  builder->tail_count = 1;
}

void set_tails(struct CallGraphBuilder *builder, int id) {
  builder->tails[0] = id;
  builder->tail_count = 1;
}

struct CursorList get_children(CXCursor cursor) {
  struct CursorList list = {.count = 0};
  clang_visitChildren(cursor, collect_children_visitor, &list);
  return list;
}

void merge_tails(struct CallGraphBuilder *builder, int *other_tails,
                 int other_count) {
  for (int i = 0; i < other_count && builder->tail_count < 32; i++) {
    builder->tails[builder->tail_count++] = other_tails[i];
  }
}

void analyze_cfg_node(CXCursor cursor, struct CallGraphBuilder *builder) {
  enum CXCursorKind kind = clang_getCursorKind(cursor);
  struct CursorList children = get_children(cursor);

  if (kind == CXCursor_CompoundStmt) {
    for (int i = 0; i < children.count; i++) {
      analyze_cfg_node(children.cursors[i], builder);
    }
  } else if (kind == CXCursor_IfStmt) {
    int if_id = clang_hashCursor(cursor) % 10000000;
    addVertex(&builder->CFG, if_id);
    setVertexLabel(&builder->CFG, if_id, "If");

    link_tails(builder, if_id);

    int then_tails[32];
    int then_count = 0;
    int else_tails[32];
    int else_count = 0;

    int then_idx = -1, else_idx = -1;
    if (children.count >= 3) {
      then_idx = children.count - 2;
      else_idx = children.count - 1;
    } else if (children.count == 2) {
      then_idx = 1;
    } else if (children.count == 1) {
      then_idx = 0;
    }

    if (then_idx != -1) {
      set_tails(builder, if_id);
      analyze_cfg_node(children.cursors[then_idx], builder);
      memcpy(then_tails, builder->tails, sizeof(int) * builder->tail_count);
      then_count = builder->tail_count;
    }

    if (else_idx != -1) {
      set_tails(builder, if_id);
      analyze_cfg_node(children.cursors[else_idx], builder);
      memcpy(else_tails, builder->tails, sizeof(int) * builder->tail_count);
      else_count = builder->tail_count;
    } else {
      else_tails[0] = if_id;
      else_count = 1;
    }

    builder->tail_count = 0;
    merge_tails(builder, then_tails, then_count);
    merge_tails(builder, else_tails, else_count);

  } else if (kind == CXCursor_ForStmt || kind == CXCursor_WhileStmt ||
             kind == CXCursor_DoStmt) {
    int loop_id = clang_hashCursor(cursor) % 10000000;
    addVertex(&builder->CFG, loop_id);

    char label[64];
    if (kind == CXCursor_ForStmt)
      strcpy(label, "For");
    else if (kind == CXCursor_WhileStmt)
      strcpy(label, "While");
    else
      strcpy(label, "DoWhile");
    setVertexLabel(&builder->CFG, loop_id, label);

    link_tails(builder, loop_id);

    if (children.count >= 1) {
      analyze_cfg_node(children.cursors[children.count - 1], builder);
      link_tails(builder, loop_id);
    }

    set_tails(builder, loop_id);

  } else if (kind == CXCursor_CallExpr || kind == CXCursor_DeclStmt ||
             kind == CXCursor_BinaryOperator ||
             kind == CXCursor_UnaryOperator || kind == CXCursor_ReturnStmt) {

    // UNIQUE NODE ID - Use clang_hashCursor to guarantee unique nodes for pure CFG behavior.
    int node_id = clang_hashCursor(cursor) % 10000000;

    char label[256];
    if (kind == CXCursor_CallExpr) {
      CXString spelling = clang_getCursorSpelling(cursor);
      CXSourceLocation loc = clang_getCursorLocation(cursor);
      unsigned line;
      clang_getSpellingLocation(loc, NULL, &line, NULL, NULL);
      
      // Append line number so it's clear these are distinct call sites, not merged functions
      snprintf(label, sizeof(label), "Call: %s (L%u)", clang_getCString(spelling), line);
      clang_disposeString(spelling);
    } else if (kind == CXCursor_DeclStmt) {
      snprintf(label, sizeof(label), "Decl");
    } else if (kind == CXCursor_ReturnStmt) {
      snprintf(label, sizeof(label), "Return");
    } else {
      snprintf(label, sizeof(label), "Expr");
    }
    addVertex(&builder->CFG, node_id);
    setVertexLabel(&builder->CFG, node_id, label);

    link_tails(builder, node_id);

    // HYBRID EDGE: If this is an internal function call, draw an edge to the function's entry node
    if (kind == CXCursor_CallExpr) {
      CXCursor referenced = clang_getCursorReferenced(cursor);
      CXSourceLocation ref_loc = clang_getCursorLocation(referenced);
      if (clang_Location_isFromMainFile(ref_loc)) {
        CXString spelling = clang_getCursorSpelling(cursor);
        const char *func_name = clang_getCString(spelling);
        int func_id = hash_string(func_name) % 10000000;
        
        // Ensure the function entry vertex exists (in case it hasn't been parsed yet)
        addVertex(&builder->CFG, func_id);
        setVertexLabel(&builder->CFG, func_id, (char*)func_name);
        
        // Draw the hybrid call edge
        addEdgeDirected(&builder->CFG, node_id, func_id, 1);
        clang_disposeString(spelling);
      }
    }
  }
}

enum CXChildVisitResult dfg_visitor(CXCursor cursor, CXCursor parent,
                                    CXClientData client_data) {
  struct CallGraphBuilder *builder = (struct CallGraphBuilder *)client_data;
  enum CXCursorKind kind = clang_getCursorKind(cursor);

  CXSourceLocation loc = clang_getCursorLocation(cursor);
  if (!clang_Location_isFromMainFile(loc)) {
    return CXChildVisit_Continue;
  }

  if (kind == CXCursor_VarDecl || kind == CXCursor_ParmDecl) {
    CXString spelling = clang_getCursorSpelling(cursor);
    const char *name = clang_getCString(spelling);
    if (strlen(name) > 0) {
      int var_id = hash_string(name) % 10000000;
      addVertex(&builder->DFG, var_id);
      char label[256];
      snprintf(label, sizeof(label), "Var: %s", name);
      setVertexLabel(&builder->DFG, var_id, label);
    }
    clang_disposeString(spelling);
  } else if (kind == CXCursor_DeclRefExpr) {
    // Ensure this is a variable reference and not a function call reference
    CXCursor referenced = clang_getCursorReferenced(cursor);
    enum CXCursorKind refKind = clang_getCursorKind(referenced);

    if (refKind == CXCursor_VarDecl || refKind == CXCursor_ParmDecl) {
      CXString spelling = clang_getCursorSpelling(cursor);
      const char *name = clang_getCString(spelling);
      if (strlen(name) > 0) {
        int var_id = hash_string(name) % 10000000;
        int expr_id = clang_hashCursor(cursor) % 10000000;

        addVertex(&builder->DFG, expr_id);
        char label[256];
        snprintf(label, sizeof(label), "Ref: %s", name);
        setVertexLabel(&builder->DFG, expr_id, label);

        addEdgeDirected(&builder->DFG, var_id, expr_id, 1);
      }
      clang_disposeString(spelling);
    }
  }

  return CXChildVisit_Recurse;
}

enum CXChildVisitResult cfg_visitor(CXCursor cursor, CXCursor parent,
                                    CXClientData client_data) {
  struct CallGraphBuilder *builder = (struct CallGraphBuilder *)client_data;

  CXSourceLocation loc = clang_getCursorLocation(cursor);
  if (!clang_Location_isFromMainFile(loc)) {
    return CXChildVisit_Continue;
  }

  if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
    CXString spelling = clang_getCursorSpelling(cursor);
    const char *name = clang_getCString(spelling);

    // Functions maintain singleton identities as CFG Execution Roots
    int func_id = hash_string(name) % 10000000;

    addVertex(&builder->CFG, func_id);
    setVertexLabel(&builder->CFG, func_id, (char *)name);
    clang_disposeString(spelling);

    builder->current_func_entry = func_id;
    builder->tail_count = 0;

    struct CursorList children = get_children(cursor);
    for (int i = 0; i < children.count; i++) {
      analyze_cfg_node(children.cursors[i], builder);
    }
  }

  return CXChildVisit_Continue;
}

struct CallGraphBuilder createFlowGraphs(char *ccode) 
{
	struct CallGraphBuilder builder;
  builder.CFG = newGraph("CFG");
  builder.DFG = newGraph("DFG");

	CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, ccode, NULL, 0, NULL, 0, CXTranslationUnit_None);

  if (unit == NULL) {
    printf("Unable to parse translation unit. Quitting.\n");
		exit(0);
	}

  CXCursor cursor = clang_getTranslationUnitCursor(unit);

  // Execute Parallel Passes
  clang_visitChildren(cursor, cfg_visitor, &builder);
  clang_visitChildren(cursor, dfg_visitor, &builder);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);

	return builder;
}

void analyzeAllCoupling(struct nGraph *G) {
  struct nGraphList *list = BronKerboschMax(G);
  nGraphListFree(list);
}

void analyzeSpaghettiCode(struct nGraph *G) {
  double density = (double)G->E->count / G->V->count;
  if (density > 2) {
    printf("[Spaghetti Code] Density is very high (%.2f). Code lacks clear "
           "layering.\n",
           density);
  }
}

void analyzeTightCoupling(struct nGraph *G) {
  int tight_coupling = 0;

  // tight coupling check (Jaccard > 0.8)
  struct edge *e = G->E->head;
  while (e != NULL) {
    double jc = jaccardCoefficient(G, e->head, e->tail);
    if (jc > 0.8) {
      printf(
          "[Tight Coupling] High similarity (%.2f) between nodes %d and %d\n",
          jc, e->head, e->tail);
      printf("               > %s\n", getVertexLabelString(G, e->head));
      printf("               > %s\n", getVertexLabelString(G, e->tail));
      tight_coupling++;
    }
    e = e->next;
  }

  printf("Tightly Coupled Pairs: %d\n", tight_coupling);
}

void analyzeDeadCode(struct nGraph *G) {
  int dead_code = 0;

  struct vertex *tmp = G->V->head;
  while (tmp != NULL) {
    if (tmp->degree_in == 0 && tmp->degree_out == 0) {
      printf("[Dead Code] Unreachable and unused vertex:\n");
      printf("          > %s\n", tmp->lblString ? tmp->lblString : "");
      tmp->color = 9; // grey
      dead_code++;
    }
    tmp = tmp->next;
  }

  printf("Dead Code: %d\n", dead_code);
}

void analyzeGodObjects(struct nGraph *G) {
  if (G->V->count == 0) {
    printf("No Vertices in Graph\n");
    return;
  }

  // average degree in vs degree out
  double total_in = 0, total_out = 0;
  struct vertex *tmp = G->V->head;
  while (tmp != NULL) {
    total_in += tmp->degree_in;
    total_out += tmp->degree_out;
    tmp = tmp->next;
  }
  double avg_in = total_in / G->V->count;
  double avg_out = total_out / G->V->count;

  // standard deviation in vs standard deviation out
  double var_in = 0, var_out = 0;
  tmp = G->V->head;
  while (tmp != NULL) {
    var_in += (tmp->degree_in - avg_in) * (tmp->degree_in - avg_in);
    var_out += (tmp->degree_out - avg_out) * (tmp->degree_out - avg_out);
    tmp = tmp->next;
  }
  double std_in = sqrt(var_in / G->V->count);
  double std_out = sqrt(var_out / G->V->count);

  // assumes normal distribution (3 - 99\%, 2 - 95\%)
  double threshold_in = avg_in + 2.0 * std_in;
  double threshold_out = avg_out + 2.0 * std_out;
  if (threshold_in < 3)
    threshold_in = 3;
  if (threshold_out < 3)
    threshold_out = 3;

  // analysis display start
  printf("\n--- Code God Objects for Graph %s = (V: %d, E: %d) ---\n", G->label,
         G->V->count, G->E->count);

  int god_objects = 0;

  tmp = G->V->head;
  while (tmp != NULL) {
    if (tmp->degree_in > threshold_in || tmp->degree_out > threshold_out) {
      printf("[God Object] High fan-in/fan-out (in:%d, out:%d)\n",
             tmp->degree_in, tmp->degree_out);
      printf("           > %s\n", tmp->lblString ? tmp->lblString : "");
      tmp->color = 0; // red
      god_objects++;
    }
    tmp = tmp->next;
  }

  printf("Total God Objects: %d\n", god_objects);
}
