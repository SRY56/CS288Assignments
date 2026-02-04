#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 256
#define MAX_LEN 256

int main() {
	FILE *inputFile, *outputFile;
	char *lines[MAX_LINES];
	char buffer[MAX_LEN];
	int count = 0;

	// open input file (env.txt)
	inputFile = fopen("env.txt", "r");
	if (inputFile == NULL) {
		printf("Error: Could not open env.txt");
		return 1;
	}

	// read each line
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL && count < MAX_LINES) {
		buffer[strcspn(buffer, "\n")] = '\0';
		lines[count] = malloc(strlen(buffer) + 1);
		strcpy(lines[count], buffer);
		count++;
	}
	fclose(inputFile);

	// Bubble sort by variable name
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - i; j++) {
			char temp1[MAX_LEN], temp2[MAX_LEN];
			strcpy(temp1, lines[j]);
			strcpy(temp1, lines[j+1]);


			char *name1 = strtok(temp1, "=");
			char *name2 = strtok(temp2, "=");

			if (name1 && name2) {
				if (strcmp(lines[j], lines[j+1]) > 0) {
					char *temp = lines[j];
					lines[j] = lines[j + 1];
					lines[j+1] = temp;
				}
			}
		}
	}

	// print sorted result to terminal
	printf("sorted Environment Variables:\n");
	printf("=========================================\n\n");
	for (int i = 0; i < count; i++) {
		printf("%s\n", lines[i]);
	}


	// write sorted results to output file
	outputFile = fopen("sorted_env.txt", "w");
	if (outputFile == NULL) {
		printf("Error creating sorted_env.txt");
		return 1;
	}

	for (int i = 0; i < count; i++) {
		fprintf(outputFile, "%s\n", lines[i]);
		free(lines[i]);
	}

	fclose(outputFile);
	return 0;
}
