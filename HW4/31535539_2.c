#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *values;
	int *column_indices;
	int *row_start;
	int num_rows;
	int num_nonzero;
} SparseMatrix;

// function to read a sparse matric from a file in CSR format
SparseMatrix read(const char *filename) {
	SparseMatrix matrix;
	FILE *file = fopen(filename, "r");

	if (file == NULL) {
		printf("Error: could not open file %s\n", filename);
		exit(1);
	}

	// read number or rows and non-zero values
	fscanf(file, "%d", &matrix.num_rows);
	fscanf(file, "%d", &matrix.num_nonzero);

	// allocate memory to arrays
	matrix.values = (int *)malloc(matrix.num_nonzero * sizeof(int));
	matrix.column_indices = (int *)malloc(matrix.num_nonzero * sizeof(int));
	matrix.row_start = (int *)malloc((matrix.num_rows +1) *sizeof(int));

	// read values array
	for (int i = 0; i < matrix.num_nonzero; i++) {
		fscanf(file, "%d", &matrix.values[i]);
	}

	//read column_indices array
	for (int i = 0; i < matrix.num_nonzero; i++) {
		fscanf(file, "%d", &matrix.column_indices[i]);
	}

	// read row_start array
	for (int i = 0; i <= matrix.num_rows; i++) {
		fscanf(file, "%d", &matrix.row_start[i]);
	}

	fclose(file);
	return matrix;
}

// function to aff two sparse matricies
SparseMatrix add(SparseMatrix m1, SparseMatrix m2) {
	SparseMatrix result;
	result.num_rows = m1.num_rows;

	// temporary arrays to store result
	int *temp_values = (int *)malloc((m1.num_nonzero + m2.num_nonzero) * sizeof(int));
	int *temp_column_indices = (int *)malloc((m1.num_nonzero + m2.num_nonzero) * sizeof(int));
	result.row_start = (int *)malloc((result.num_rows + 1) * sizeof(int));
	
	int result_index = 0;
	result.row_start[0] = 0;

	// process each row
	for (int row = 0; row < result.num_rows; row++) {
		int i = m1.row_start[row];
		int i_end = m1.row_start[row + 1];
		int j = m2.row_start[row];
		int j_end = m2.row_start[row + 1];

		while (i < i_end && j < j_end) {
			int col1 = m1.column_indices[i];
			int col2 = m2.column_indices[j];

			if (col1 < col2) {
				// Element only in matrix 1
				temp_values[result_index] = m1.values[i];
				temp_column_indices[result_index] = col1;
				result_index++;
				i++;
			} else if (col2 < col1) {
				// element only in matrix 2
				temp_values[result_index] = m2.values[j];
				temp_column_indices[result_index] = col2;
				result_index++;
				j++;
			} else {
				// elements at the same column. add
				int sum = m1.values[i] + m2.values[j];
				// store in sum is nonzero
				if (sum != 0) {
					temp_values[result_index] = sum;
					temp_column_indices[result_index] = col1;
					result_index++;
				}
				i++;
				j++;
			}
		}

		// add remaining elements from matrix 1
		while (i < i_end) {
			temp_values[result_index] = m1.values[i];
			temp_column_indices[result_index] = m1.column_indices[i];
			result_index++;
			i++;
		}

		while (j < j_end) {
			temp_values[result_index] = m2.values[j];
			temp_column_indices[result_index] = m2.column_indices[j];
			result_index++;
			j++;
		}
		result.row_start[row + 1] = result_index;
	}

	// allocate memory needed and copy data
	result.num_nonzero = result_index;
	result.values = (int *)malloc(result.num_nonzero * sizeof(int));
	result.column_indices = (int *)malloc(result.num_nonzero *sizeof(int));

	for (int i = 0; i < result.num_nonzero; i++) {
		result.values[i] = temp_values[i];
		result.column_indices[i] = temp_column_indices[i];
	}

	free(temp_values);
	free(temp_column_indices);

	return result;
}

// function to print sparse matrix in CSR format
void printCSR(SparseMatrix matrix) {
	printf("Number of rows: %d\n", matrix.num_rows);
	printf("Number of non-zero values: %d\n", matrix.num_nonzero);

	printf("Values Array: ");
	for (int i = 0; i < matrix.num_nonzero; i++) {
		printf("%d ", matrix.values[i]);
	}
	printf("\n");

	printf("Column indices array: ");
	for (int i = 0; i < matrix.num_nonzero; i++) {
		printf("%d ", matrix.column_indices[i]);
	}
	printf("\n");

	printf("Row start array: ");
	for (int i = 0; i <= matrix.num_rows; i++) {
		printf("%d ", matrix.row_start[i]);
	}
	printf("\n");
}

// function to write sparse matrix to file in CSR format
void writeSparseMatrix(const char *filename, SparseMatrix matrix) {
	FILE *file = fopen(filename, "w");

	if (file == NULL) {
		printf("Error: could not open file %s for writing\n", filename);
		exit(1);
	}

	//write number or rows
	fprintf(file, "%d\n", matrix.num_rows);

	// wirte number of non-zero values
	fprintf(file, "%d\n", matrix.num_nonzero);

	// write values array
	for (int i = 0; i < matrix.num_nonzero; i++) {
		fprintf(file, "%d\n", matrix.values[i]);
	}

	// write column_indices array
	for (int i = 0; i < matrix.num_nonzero; i++) {
		fprintf(file, "%d\n", matrix.column_indices[i]);
	}

	// write row_start array
	for (int i = 0; i <= matrix.num_rows; i++) {
		fprintf(file, "%d\n", matrix.row_start[i]);
	}

	fclose(file);
}

// function to free memory
void freeMatrix(SparseMatrix matrix) {
	free(matrix.values);
	free(matrix.column_indices);
	free(matrix.row_start);
}


int main() {
	// read the two sparse matricies
	SparseMatrix matrix1 = read("sparse_matrix1.txt");
	SparseMatrix matrix2 = read("sparse_matrix2.txt");

	// print matrix 1 in CSR format
	printf("Sparse Matrix 1\n");
	printf("------------------------------------\n");
	printCSR(matrix1);
	printf("\n");

	// print matrix 2 in CSR format
	printf("Sparse Matrix 2\n");
	printf("------------------------------------\n");
	printCSR(matrix2);
	printf("\n");
	
	// add the matriciee
	SparseMatrix result = add(matrix1, matrix2);

	// print result in CSR format
	printf("Result\n");
	printf("-----------------------------------\n");
	printCSR(result);
	printf("\n");

	// write result in file
	writeSparseMatrix("sparse_result.txt", result);

	// free memory
	freeMatrix(matrix1);
	freeMatrix(matrix2);
	freeMatrix(result);

	return 0;
}
