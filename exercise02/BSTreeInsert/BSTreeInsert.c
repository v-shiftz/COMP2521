
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

static BSTree newBSTNode(int value) {
	BSTree t = malloc(sizeof(*t));
	if (t == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
	t->value = value;
	t->left = NULL;
	t->right = NULL;
	return t;
}

BSTree BSTreeInsert(BSTree t, int val) {
	// BSTree lmao = t;
	if (t == NULL) {
		return newBSTNode(val);
	}
	if (t -> value > val) {
		t -> left = BSTreeInsert(t -> left, val);
	} else if (t -> value < val) {
		t -> right = BSTreeInsert(t -> right, val);
	}
	// while (1) {
	// 	if (lmao -> value > val) {
	// 		if (lmao -> left != NULL) {
	// 			lmao = lmao -> left;
	// 		} else {
	// 			lmao -> left = newBSTNode(val);
	// 		}
	// 	} else if (lmao -> value < val) {
	// 		if (lmao -> right != NULL) {
	// 			lmao = lmao -> right;
	// 		} else {
	// 			lmao -> right = newBSTNode(val);
	// 		}
	// 	} else {
	// 		break;
	// 	}
	// }
	return t;
}

