
#include <stdlib.h>

#include "BSTree.h"

int BSTreeNodeDepth(BSTree t, int key) {
	int depth;
	if (t == NULL) {
		return -1;
	} else if (key == t -> value) {
		return 0;
	} else if (key < t -> value){
		depth = BSTreeNodeDepth(t -> left, key);
		if (depth != -1) {
			return depth += 1;
		} else {
			return - 1;
		}
	} else {
		depth = BSTreeNodeDepth(t -> right, key);
		if (depth != -1) {
			return depth += 1;
		} else {
			return - 1;
		}
	}
}

