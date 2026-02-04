#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *f;
    int dim, element;
    char filename[256];
    
    printf("Provide a filename: ");
    scanf("%s", filename);
    
    f = fopen(filename, "wb");
    if (f == NULL) {
        printf("ERROR: File could not be opened.\n");
        return 1;
    }
    
    printf("Provide a size 'n' for the matrix (n x n): ");
    scanf("%d", &dim);
    
    if (dim <= 0 || dim > 100) {
        printf("ERROR: Matrix dimension must be between 1 and 100.\n");
        fclose(f);
        return 1;
    }
    
    printf("dim = %d\n", dim);
    
    // Write dimension to file
    fwrite(&dim, sizeof(dim), 1, f);
    
    // Seed with current time for different data each run
    srandom(time(NULL));
    
    // Generate and write random matrix elements
    for (int i = 0; i < dim * dim; i++) {
        element = random() % 10;  // Random values 0-9
        printf("element %d: %d\n", i, element);
        fwrite(&element, sizeof(element), 1, f);
    }
    
    fclose(f);
    printf("Matrix successfully written to %s\n", filename);
    
    return 0;
}
