
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"

int findMissingInteger(int *integers, int n);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "usage: %s <numbers>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = argc;
	int *integers = malloc((n - 1) * sizeof(int));
	for (int i = 1; i < argc; i++) {
		integers[i - 1] = atoi(argv[i]);
	}

	printf("The missing integer is %d\n", findMissingInteger(integers, n));

	free(integers);
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Given an array that contains all of the integers from 1 to n with exactly
 * one of them missing, returns that missing integer. For example, given the
 * array [1, 6, 2, 5, 3] (i.e. n = 6), the missing integer is 4.
 *
 * You may assume that the array is non-empty. Moreover, the function's
 * behaviour is considered undefined for arrays other than those satisfying the
 * property above.
 */

int findMissingInteger(int *integers, int n) {
	HashTable table = HashTableNew();
	for (int i = 0; i < n - 1; i++) {
        	HashTableInsert(table, integers[i], 0);
	}
	
	// Find the missing integer by checking each number from 1 to n
	int missing = -1;
	for (int i = 1; i <= n; i++) {
		if (!HashTableContains(table, i)) {
		missing = i;
		break;
		}
	}
	
	HashTableFree(table);
	return missing;
}

