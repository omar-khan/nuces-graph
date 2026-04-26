#include "nucesGraph.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Specify dot file to load\n");
		return -1;
	}

	struct nGraph G = importDot("G", argv[1]);

	analyzeGodObjects(&G);
	analyzeDeadCode(&G);
	analyzeTightCoupling(&G);
	analyzeAllCoupling(&G);
	analyzeSpaghettiCode(&G);

	//show(&G);
	showDot(&G);

	return 0;
}
