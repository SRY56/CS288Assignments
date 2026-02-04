#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INT 100
#define NUM_BUCKETS 16
#define BITS_PER_PASS 4
#define NUM_PASSES 8

void radixSort(int32_t *arr, int n) {
	int32_t *output = (int32_t *)malloc(n * sizeof(int32_t));
	int count[NUM_BUCKETS];

	// perform 8 passes
	for (int pass = 0; pass < NUM_PASSES; pass++) {
		int shift = pass * BITS_PER_PASS;

		// initialize count array
		for (int i = 0; i < NUM_BUCKETS; i++) {
			count[i] = 0;
		}

		// count occurences of each digit
		for (int i = 0; i < n; i++) {
			uint32_t value = (uint32_t)arr[i];
			// last pass (most significant digit) flip sihn bit
			// make negative numbers come before positive numbers
			if (pass == NUM_PASSES - 1) {
				value ^=0x80000000;
			}
			int digit = (value >> shift) & 0xF;
			count[digit]++;
		}

		// calculate cumulative count
		for (int i = 1; i < NUM_BUCKETS; i++) {
			count[i] += count[i - 1];
		}

		// build output array
		// process from right to left
		for (int i = n - 1; i >= 0; i--) {
			uint32_t value = (uint32_t)arr[i];
			// flip sign bit for last pass
			if (pass == NUM_PASSES - 1) {
				value ^= 0x80000000;
			}
			int digit = (value >> shift) & 0xF;
			output[count[digit] - 1] = arr[i];
			count[digit]--;
		}

		// copy output back to arr
		for (int i = 0; i < n; i++) {
			arr[i] = output[i];
		}
	}

	free(output);
}

int main() {
	int count;
	int32_t arr[MAX_INT];

	// read count
	if (scanf("%d", &count) != 1) {
		return 1;
	}

	// read integers
	for (int i = 0; i < count; i++) {
		if (scanf("%d", &arr[i]) != 1) {
			return 1;
		}
	}

	// sort hexadecimal
	radixSort(arr, count);

	// print sorted integers
	for (int i = 1; i < count; i++) {
		printf("%d\n", arr[i]);
	}

	return 0;
}

