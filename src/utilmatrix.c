#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nucesGraph.h"

void matrix_vector_multiply(struct nGraph *G, double *vector, double *result) {
    int i, j;
    for (i = 0; i < G->V->count; i++) {
        result[i] = 0.0;
        for (j = 0; j < G->V->count; j++) {
            result[i] += G->laplacian_matrix[i * G->V->count + j] * vector[j];
        }
    }
}

double normalize_vector(double *vector, int N) {
    int i;
    double norm = 0.0;
    for (i = 0; i < N; i++) {
        norm += vector[i] * vector[i];
    }
    norm = sqrt(norm);
    for (i = 0; i < N; i++) {
        vector[i] /= norm;
    }
    return norm;
}

void power_method(struct nGraph *G, double *eigenvalues) {
    int i, j, k;
    double *x = (double *)malloc(G->V->count * sizeof(double));
    double *y = (double *)malloc(G->V->count * sizeof(double));
    double *z = (double *)malloc(G->V->count * sizeof(double));

		double lambda, norm;
    // Initialize x to a random unit vector
    for (i = 0; i < G->V->count; i++) {
        x[i] = rand()/(double)RAND_MAX;
    }

		for (i = 0; i < G->V->count; i++) {
			for (j = 0; j < 1000; j++) { // Max Iterations
				normalize_vector(x, G->V->count);
				matrix_vector_multiply(G, x, y);

				lambda = 0;
				for (k = 0; k < G->V->count; k++) {
					lambda += y[k] * x[k];
				}
				eigenvalues[i] = lambda;

				matrix_vector_multiply(G, y, z);
				norm = normalize_vector(z, G->V->count);
				if (fabs(norm - 1) < 1e-4) break;

				memcpy(x, z, sizeof(double)*G->V->count);
			}
			for (k = 0; k <= i; k++) {
				G->laplacian_matrix[k * G->V->count + k] -= eigenvalues[i];
			}
		}
    free(x);
    free(y);
		free(z);
}

void qr_decomposition(struct nGraph *G, double *Q, double *R) {
    double *v = (double *)calloc(G->V->count, sizeof(double));
    double *w = (double *)calloc(G->V->count, sizeof(double));

    // Copy the input matrix to R
		memcpy(R, G->laplacian_matrix, sizeof(double)*G->V->count*G->V->count);

    // Initialize Q to the identity matrix
    int i;
    for (i = 0; i < G->V->count; i++) {
			Q[i * G->V->count + i] = 1.0;
    }

    double alpha, norm;
    // Perform the QR decomposition of R
		int j, k;
    for (j = 0; j < G->V->count - 1; j++) {
        // Compute the norm of the j-th column of R
        norm = 0.0;
        for (i = j; i < G->V->count; i++) {
            norm += R[i * G->V->count + j] * R[i * G->V->count + j];
        }
        norm = sqrt(norm);

        // Compute the j-th Householder vector v
        if (R[j * G->V->count + j] >= 0.0) {
            alpha = -norm;
        } else {
            alpha = norm;
        }

        v[j] = R[j * G->V->count + j] - alpha;
        for (i = j + 1; i < G->V->count; i++) {
            v[i] = R[i * G->V->count + j];
        }
        norm = normalize_vector(v, G->V->count);

        // Compute the j-th Householder matrix H
        for (i = 0; i < G->V->count; i++) {
            for (k = j; k < G->V->count; k++) {
                w[i] = v[i] * R[k * G->V->count + j];
            }
            for (k = j; k < G->V->count; k++) {
                R[k * G->V->count + i] -= 2.0 * v[k] * w[i];
            }
            for (k = 0; k < G->V->count; k++) {
                Q[k * G->V->count + i] -= 2.0 * v[k] * w[i];
            }
        }
    }
    // Free the allocated memory
    free(v);
    free(w);
}




