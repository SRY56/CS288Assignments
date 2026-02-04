#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROWS 5
#define COLS 5
#define RATIO 0.2 // The probability that a given element is non-zero
void generate_sparse(char *filename) {
    FILE *file = fopen(filename, "w");
    int n = ROWS;
    int m = 0;
    int matrix[ROWS][COLS];
    int values[ROWS * COLS];
    int col_indices[ROWS * COLS];
    int row_start[ROWS + 1];
    // Generate dense matrix and count non-zero values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < COLS; j++) {
            if ((double)rand() / (double)RAND_MAX < RATIO) {
                matrix[i][j] = rand() % 10 + 1; // Non-zero value
                m++;
            } else {
                matrix[i][j] = 0; // Zero value
            }
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    // Write CSR representation of matrix to file
    int count = 0;
    row_start[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] != 0) {
                values[count] = matrix[i][j];
                col_indices[count] = j;
                count++;
            }
        }
        row_start[i+1] = count;
    }
    fprintf(file, "%d\n%d\n", n, m);
    for (int i = 0; i < m; i++) {
        fprintf(file, "%d\n", values[i]);
    }
    for (int i = 0; i < m; i++) {
        fprintf(file, "%d\n", col_indices[i]);
    }
    for (int i = 0; i <= n; i++) {
        fprintf(file, "%d\n", row_start[i]);
    }
    fclose(file);
}

int main() {
    srand(time(NULL)); // Initialize random number generator here instead of in generate_sparse()
    printf("Sparse matrix 1\n------------------------------------\n");
    generate_sparse("sparse_matrix1.txt");
    printf("Sparse matrix 2\n------------------------------------\n");
    generate_sparse("sparse_matrix2.txt");
    return 0;
}
