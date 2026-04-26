#include <stdio.h>
#include "nucesGraph.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
    printf("Usage: %s <source_file.c>\n", argv[0]);
    return 1;
  }

	struct CallGraphBuilder builder = createFlowGraphs(argv[1]);

	showDot(&builder.CFG);
  showDot(&builder.DFG);

	clearFlowGraphs(&builder);

	return 0;
}
