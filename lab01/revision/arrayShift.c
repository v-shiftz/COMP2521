
#include <stdio.h>
#include <stdlib.h>

void shift(int *arr, int size, int n);

void printArray(int *arr, int size);

int main(void) {
	printf("Enter array size: ");
	int size = 0;
	if (scanf("%d", &size) != 1) {
		fprintf(stderr, "error: failed to read array size\n");
		exit(EXIT_FAILURE);
	} else if (size < 0) {
		fprintf(stderr, "error: invalid array size\n");
		exit(EXIT_FAILURE);
	}

	int *arr = malloc(size * sizeof(int));
	if (arr == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	if (size > 0) {
		printf("Enter array values: ");
	}
	for (int i = 0; i < size; i++) {
		if (scanf("%d", &arr[i]) != 1) {
			fprintf(stderr, "error: failed to read array value\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("Array: ");
	printArray(arr, size);

	printf("Enter shift: ");
	int n = 0;
	if (scanf("%d", &n) != 1) {
		fprintf(stderr, "error: failed to read shift\n");
		exit(EXIT_FAILURE);
	} else if (n < 0) {
		fprintf(stderr, "error: invalid shift\n");
		exit(EXIT_FAILURE);
	}

	shift(arr, size, n);
	printf("Array after shifting %d time(s): ", n);
	printArray(arr, size);

	free(arr);
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

void shift(int *arr, int size, int n) {
	n = n % size;
    if (n == 0 || size <= 1) { // If n is 0 or the array size is 0 or 1, no need to shift
        return;
    }

    int *temp = malloc(size * sizeof(int)); // Temporary array to hold the shifted elements
    if (temp == NULL) {
		fprintf(stderr, "error: memory allocation error\n"); // Print error message if memory allocation failure
		exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) { // Calculate the new position for each element after shifting
        int newPos = (i + n) % size;
        temp[newPos] = arr[i];
    }

    for (int i = 0; i < size; i++) { // Move the shifted elements back into the original array
        arr[i] = temp[i];
    }

    free(temp); // Free the temporary array
}

////////////////////////////////////////////////////////////////////////
// !!! DO NOT MODIFY THE CODE BELOW !!!

void printArray(int *arr, int size) {
	printf("[");
	for (int i = 0; i < size; i++) {
		if (i > 0) {
			printf(", ");
		}
		printf("%d", arr[i]);
	}
	printf("]\n");
}

