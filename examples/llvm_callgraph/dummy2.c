#include <stdio.h>

void multiplyMatrices(int first[10][10], int second[10][10], int result[10][10], int r1, int c1, int r2, int c2) {
    int i, j, k;

    // Initialize result matrix to zero
    for (i = 0; i < r1; ++i) {
        for (j = 0; j < c2; ++j) {
            result[i][j] = 0;
        }
    }

    // Multiply first and second matrices and store in result
    for (i = 0; i < r1; ++i) {
        for (j = 0; j < c2; ++j) {
            for (k = 0; k < c1; ++k) {
                result[i][j] += first[i][k] * second[k][j];
            }
        }
    }
}

int main() {
    int first[10][10] = { {2, 4}, {3, 4} };
    int second[10][10] = { {1, 2}, {1, 3} };
    int result[10][10];
    
    int i, j;

    multiplyMatrices(first, second, result, 2, 2, 2, 2);
    
    // Displaying the result
    printf("Output Matrix:\n");
    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 2; ++j) {
            printf("%d  ", result[i][j]);
            if (j == 1) {
                printf("\n");
            }
        }
    }

    return 0;
}
