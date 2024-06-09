
#include "tree.h"

static Tree newBSTNode(int value) {
	Tree t = malloc(sizeof(*t));
	if (t == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
	t->value = value;
	t->left = NULL;
	t->right = NULL;
	return t;
}

Tree TreeCopy(Tree t, int depth) {
	if (t == NULL || depth == -1) {
		return NULL;
	}
	Tree new = newBSTNode(t -> value);
	new -> left = TreeCopy(t -> left, depth - 1);
	new -> right = TreeCopy(t -> right, depth - 1);
	return new;
}

