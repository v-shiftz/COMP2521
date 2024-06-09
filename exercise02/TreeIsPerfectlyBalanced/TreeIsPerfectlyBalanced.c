
#include <stdlib.h>

#include "tree.h"

int getHeight(Tree t) {
	if (t == NULL) {
		return 0;
	} else {
		return 1 + getHeight(t -> left) + getHeight(t -> right);
	}
}

bool TreeIsPerfectlyBalanced(Tree t) {
	if (t == NULL) {
		return true;
	}
	int left = getHeight(t -> left);
	int right = getHeight(t -> right);
	if (left - right > 1 || right - left > 1) {
		return false;
	} else {
		return TreeIsPerfectlyBalanced(t -> left) && TreeIsPerfectlyBalanced(t -> right);
	}
}

