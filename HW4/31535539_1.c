#include <stdio.h>
#include <stdlib.h>

void print_matrix(const char *name, int matrix[100][100], int n) {
        printf("\n%s (%d x %d):\n", name, n, n);
        for (int i = 0; i < n; i++){
                for (int j = 0; j < n; j++) {
                        printf("%4d ", matrix[i][j]);
                }
                printf("\n");
        }
}

int main() {
        int n, n2;
        int matrix1[100][100], matrix2[100][100], result_matrix[100][100];
        FILE *file1, *file2, *result;

        // open matrix1.bin and matrix2.bin for reading
        file1 = fopen("matrix1.bin", "rb");
        file2 = fopen("matrix2.bin", "rb");
        if (!file1 || !file2) {
                if (file1) fclose(file1);
                if (file2) fclose(file2);
                printf("Error: Could Not Open Input Files.\n");
                return 1;
        }

        // read the dimension of the matrices
        fread(&n, sizeof(int), 1, file1);
        fread(&n2, sizeof(int), 1, file2);

        // check for compatability
        if (n != n2 || n > 100) {
                printf("Error: Matricies Are not compaible for multiplication.\n");
                fclose(file1);
                fclose(file2);
                return 1;
        }

       	int *temp = (int *)malloc(n * n * sizeof(int));
	fread(temp, sizeof(int), n * n, file1);
	for (int i = 0; i < n; i ++) {
		for ( int j = 0; i < n; j++) {
			matrix1[i][j] = temp[i *n + j];
		}
	}
	
	fread(temp, sizeof(int), n * n, file2);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix2[i][j] = temp[i * n + j];
		}
	}
	free(temp);

        // close input files
        fclose(file1);
        fclose(file2);

        // print the matricies befor multipication
        print_matrix("Matrix 1", matrix1, n);
        print_matrix("Matrix 2", matrix2, n);

        // perform matrix multipication
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        result_matrix[i][j] = 0;
                        for (int k = 0; k < n; k++) {
                                result_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
                        }
                }
        }

        // open result.bin for writing
        result = fopen("result.bin", "wb");
        if (result == NULL) {
                printf("Error: could not open result.bin for writing.\n");
                return 1;
        }

        // write the dimension for result_matrix
        fwrite(&n, sizeof(int), 1, result);

        // write the elements of result matrix
        int *temp_result = (int *) malloc(n * n * sizeof(int));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			temp_result[i * n + j] = result_matrix[i][j];
		}
	}
	fwrite(temp_result, sizeof(int), n * n, result);
	free(temp_result);

        // close the output file
        fclose(result);

        // print the result matrix
        print_matrix("Result Matrix (A x B)", result_matrix, n);

        printf("Matrix Multipication completed successfully.\n");
        return 0;
}
