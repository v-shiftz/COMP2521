
#include "list.h"

void listReverse(List l) {
	if (l == NULL) {
		exit(1);
	}
	struct node *prev = NULL;
	struct node *current = l -> head;
	struct node *next = NULL;
	while (current != NULL) {
		next = current -> next;
		current -> next = prev;
		prev = current;
		current = next;
	}
	l -> head = prev;
}

