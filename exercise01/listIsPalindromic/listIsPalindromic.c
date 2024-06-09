
#include "list.h"

bool listIsPalindromic(List l) {
	struct node *start = l -> first;
	struct node *end = l -> last;
	for (int i = 0; i < l -> size; i ++) {
		if (start -> value != end -> value) {
			return false;
		}
		start = start -> next;
		end = end -> prev;
	}
	return true;
}

