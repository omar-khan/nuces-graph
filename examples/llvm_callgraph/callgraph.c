#include "nucesGraph.h"
#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple DJB2 string hash for function deduplication
unsigned long hash_string(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

struct CallGraphBuilder {
  struct nGraph G;
  struct nGraph DFG;
  int tails[32];
  int tail_count;
  int current_func_entry;
};

struct CursorList {
  CXCursor cursors[128];
  int count;
};

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

struct CursorList get_children(CXCursor cursor) {
  struct CursorList list = {.count = 0};
  clang_visitChildren(cursor, collect_children_visitor, &list);
  return list;
}

void link_tails(struct CallGraphBuilder *builder, int target_id) {
  if (builder->tail_count == 0 && builder->current_func_entry != 0) {
    addEdgeDirected(&builder->G, builder->current_func_entry, target_id, 1);
  }
  for (int i = 0; i < builder->tail_count; i++) {
    addEdgeDirected(&builder->G, builder->tails[i], target_id, 1);
  }
  builder->tails[0] = target_id;
  builder->tail_count = 1;
}

void set_tails(struct CallGraphBuilder *builder, int id) {
  builder->tails[0] = id;
  builder->tail_count = 1;
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
    addVertex(&builder->G, if_id);
    setVertexLabel(&builder->G, if_id, "If");

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
    addVertex(&builder->G, loop_id);

    char label[64];
    if (kind == CXCursor_ForStmt)
      strcpy(label, "For");
    else if (kind == CXCursor_WhileStmt)
      strcpy(label, "While");
    else
      strcpy(label, "DoWhile");
    setVertexLabel(&builder->G, loop_id, label);

    link_tails(builder, loop_id);

    if (children.count >= 1) {
      analyze_cfg_node(children.cursors[children.count - 1], builder);
      link_tails(builder, loop_id);
    }

    set_tails(builder, loop_id);

  } else if (kind == CXCursor_CallExpr || kind == CXCursor_DeclStmt ||
             kind == CXCursor_BinaryOperator ||
             kind == CXCursor_UnaryOperator || kind == CXCursor_ReturnStmt) {

    // UNIQUE NODE ID - Reverted from deduplication to fix IfElse branch merging
    int node_id = clang_hashCursor(cursor) % 10000000;
    addVertex(&builder->G, node_id);

    char label[256];
    if (kind == CXCursor_CallExpr) {
      CXString spelling = clang_getCursorSpelling(cursor);
      snprintf(label, sizeof(label), "%s", clang_getCString(spelling));
      clang_disposeString(spelling);
    } else if (kind == CXCursor_DeclStmt) {
      snprintf(label, sizeof(label), "Decl");
    } else if (kind == CXCursor_ReturnStmt) {
      snprintf(label, sizeof(label), "Return");
    } else {
      snprintf(label, sizeof(label), "Expr");
    }
    setVertexLabel(&builder->G, node_id, label);

    link_tails(builder, node_id);
  }
}

// -------------------------------------------------------------
// DFG Visitor implementation
// -------------------------------------------------------------
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

// -------------------------------------------------------------
// Main CFG Visitor
// -------------------------------------------------------------
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

    addVertex(&builder->G, func_id);
    setVertexLabel(&builder->G, func_id, (char *)name);
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

void dump_dot(struct nGraph *G, const char *filename) {
  FILE *f = fopen(filename, "w");
  if (!f)
    return;
  fprintf(f, "digraph %s {\n", G->label);
  fprintf(f, "\toverlap = false;\n\tsplines = \"curved\";\n\tnode "
             "[shape=\"circle\"];\n");

  struct vertex *v = G->V->head;
  while (v) {
    if (v->lblString) {
      fprintf(f, "\t%d [label=\"%s\"];\n", v->label, v->lblString);
    } else {
      fprintf(f, "\t%d;\n", v->label);
    }
    v = v->next;
  }

  struct edge *e = G->E->head;
  while (e) {
    fprintf(f, "\t%d -> %d[label=\"%d\"];\n", e->head, e->tail, e->weight);
    e = e->next;
  }
  fprintf(f, "}\n");
  fclose(f);
  printf("Successfully mapped graph to %s\n", filename);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <source_file.c>\n", argv[0]);
    return 1;
  }

  struct CallGraphBuilder builder;
  builder.G = newGraph("CFG");
  builder.DFG = newGraph("DFG");

  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, argv[1], NULL, 0, NULL, 0, CXTranslationUnit_None);

  if (unit == NULL) {
    printf("Unable to parse translation unit. Quitting.\n");
    return 1;
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);

  // Execute Parallel Passes
  clang_visitChildren(cursor, cfg_visitor, &builder);
  clang_visitChildren(cursor, dfg_visitor, &builder);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);

  dump_dot(&builder.G, "cfg.dot");
  dump_dot(&builder.DFG, "dfg.dot");

  showDot(&builder.G);
  showDot(&builder.DFG);

  nGraphFree(&builder.G);
  nGraphFree(&builder.DFG);

  return 0;
}