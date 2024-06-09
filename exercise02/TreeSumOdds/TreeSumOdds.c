
#include <stdlib.h>

#include "tree.h"

int TreeSumOdds(Tree t) {
	if (t == NULL) {
		return 0;
	} else {
		if (t -> value % 2 == 1) {
			return TreeSumOdds(t -> left) + TreeSumOdds(t -> right) + t -> value;
		} else {
			return TreeSumOdds(t -> left) + TreeSumOdds(t -> right);
		}
	}
}

