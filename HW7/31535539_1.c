#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function to get maximum val in array
unsigned int getMax(unsigned int arr[], int n) {
	unsigned int max = arr[0];
	for (int i = 1; i < n; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

// count sort function for radix sort
void countingSort(unsigned int arr[], int n, unsigned int exp) {
	unsigned int output[100];
	int count[10] = {0};

	// store occurences
	for (int i = 0; i < n; i++) {
		count[(arr[i] / exp) % 10]++;
	}

	// change count[i] so it contains actual position
	for (int i = 1; i < 10; i++) {
		count[i] += count[i - 1];
	}

	// build output array
	for (int i = n - 1; i >= 0; i--) {
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}

	// copy output array tp arr[]
	for (int i = 0; i < n; i++) {
		arr[i] = output[i];
	}
}

// radix sort function
void radixSort(unsigned int arr[], int n) {
	// handle negative floats, flip all bits to signed
	// for positive, flip only the signed bit
	for (int i = 0; i < n; i++) {
		if (arr[i] & 0x80000000) {
			// negative number
			arr[i] = ~arr[i];
		} else {
			// positive number
			arr[i] ^= 0x80000000;
		}
	}

	// find maximum number
	unsigned int max = getMax(arr, n);

	// count sort for every digit
	for (unsigned int exp = 1; max / exp > 0; exp *= 10) {
		countingSort(arr, n, exp);
	}

	// restore original representation
	for (int i = 0; i < n; i++) {
		if (arr[i] & 0x80000000) {
			// positive
			arr[i] ^= 0x80000000;
		} else {
			// negative
			arr[i] = ~arr[i];
		}
	}
}

int main() {
	int count;
	float f[100];
	unsigned int temp[100];

	// ask user for count
	printf("Enter the count of numbers: ");
	if (scanf("%d", &count) != 1) {
		printf("Invalid Input\n");
		return 1;
	}

	// validate count
	if (count <=0 || count > 100) {
		printf("Count must be between 1 and 100\n");
		return 1;
	}

	// real float number from user
	printf("Enter %d float numbers:\n", count);
	for (int i = 0; i < count; i++) {
		scanf("%f", &f[i]);
	}

	// copy float data to unsigned int buffer
	memcpy(temp, f, count * sizeof(float));

	// apply radix sort
	radixSort(temp, count);

	// copy sorted data back to float array
	memcpy(f, temp, count * sizeof(float));

	// print sorted numbers (one per line)
	printf("\nSorted numbers:\n");
	for (int i = 0; i < count; i++) {
		printf("%.2f\n", f[i]);
	}

	return 0;
}
