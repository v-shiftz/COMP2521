
#include "list.h"
static void deleteMax(List l, int max);

int listDeleteLargest(List l) {
	int maxVal = 0;
	struct node *current = l -> head;
	while (current != NULL) {
		if (current -> value > maxVal) {
			maxVal = current -> value;
			current = current -> next;
		} else {
			current = current -> next;
		}
	}
	if (maxVal != 0) {
		deleteMax(l, maxVal);
	}
	return maxVal;
}

static void deleteMax(List l, int max) {
	Node prev = NULL;
	Node curr = l->head;
	while (curr->value != max) {
		prev = curr;
		curr = curr->next;
	}
	
	if (curr == l->head) {
		l->head = curr->next;
	} else {
		prev->next = curr->next;
	
	free(curr);
}

