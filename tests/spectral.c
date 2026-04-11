#include "nucesGraph.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct nGraph home = newWheel("W", 17);

	//double *eigenvalues = malloc(sizeof(double)*home.V->count);
	printLaplacianMatrix(&home);
	//power_method(&home, eigenvalues);


	double *Q = calloc(home.V->count*home.V->count, sizeof(double));
	double *R = calloc(home.V->count*home.V->count, sizeof(double));
	qr_decomposition(&home, Q, R);

	printf("\n---------\n");

	int i, j;
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			printf("%4.2f\t", Q[j*home.V->count+i]);
		}
		printf("\n");
	}
	printf("\n---------\n");
	for (i = 0; i < home.V->count; i++) {
		for (j = 0; j < home.V->count; j++) {
			printf("%4.2f\t", R[j*home.V->count+i]);
		}
		printf("\n");
	}

	return 0;
}
