
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"

bool areSimilarStrings(char *s1, char *s2);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <string 1> <string 2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bool result = areSimilarStrings(argv[1], argv[2]);
	printf("The strings %s similar\n", result ? "are" : "are not");
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Returns true if two strings are similar, and false otherwise. Two strings s1
 * and s2 are similar if, for each character in s1, it is possible to *uniquely*
 * replace it by another character (possibly itself) such that, after all
 * replacements are done to s1, we end up with s2. For example, the strings
 * "adt" and "bst" are similar, but "adt" and "dcc" are not.
 */
bool areSimilarStrings(char *s1, char *s2) {
	if (strlen(s1) != strlen(s2)) {
        return false;  // Strings of different lengths cannot be similar
    }

    int length = strlen(s1);
    HashTable table1 = HashTableNew();
    HashTable table2 = HashTableNew();

    if (!table1 || !table2) {
        fprintf(stderr, "Failed to create hash tables\n");
        if (table1){ 
			HashTableFree(table1);
		}
        if (table2){ 
			HashTableFree(table2);
		}
        return false;
    }

    for (int i = 0; i < length; i++) {
        char c1 = s1[i];
        char c2 = s2[i];

        if (HashTableContains(table1, c1)) {
            if (HashTableGet(table1, c1) != c2) {
                HashTableFree(table1);
                HashTableFree(table2);
                return false;
            }
        } else {
            HashTableInsert(table1, c1, c2);
        }

        if (HashTableContains(table2, c2)) {
            if (HashTableGet(table2, c2) != c1) {
                HashTableFree(table1);
                HashTableFree(table2);
                return false; 
            }
        } else {
            HashTableInsert(table2, c2, c1);
        }
    }

    HashTableFree(table1);
    HashTableFree(table2);
    return true;
}

