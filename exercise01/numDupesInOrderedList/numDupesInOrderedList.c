
#include "list.h"

int numDupesInOrderedList(List l) {
	struct node *current = l -> head;
	struct node *next = current -> next;
	int count = 0;
	while (next != NULL) {
		if (current -> value == next -> value) {
			count++;
		}
		current = current -> next;
		next = current -> next;
	}
	return count;
}

