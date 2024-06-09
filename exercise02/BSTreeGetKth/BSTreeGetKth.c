
#include <stdlib.h>

#include "BSTree.h"

int leftSize(BSTree t) {
	if (t == NULL) {
		return 0;
	} else {
		return 1 + leftSize(t -> left) + leftSize(t -> right);
	}
}

int BSTreeGetKth(BSTree t, int k) {
	if (t == NULL) {
		return 0;
	}
	int left = leftSize(t -> left);
	if (k == left){
		return t -> value;
	}else if (k < left){
		return BSTreeGetKth(t -> left, k);
	}else {
		return BSTreeGetKth(t -> right, k - left - 1);
	}
}

