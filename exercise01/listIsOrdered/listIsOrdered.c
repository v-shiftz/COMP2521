
#include "list.h"

bool listIsOrdered(List l) {
	if (l -> head == NULL) {
		return true;
	}
	struct node *current = l -> head;
	struct node *next = current -> next;
	bool ascending = false;
	bool decending = false;
	while (next != NULL){
		if (current -> value > next -> value){
			ascending = true;
			current = current -> next;
			next = current -> next;
		} 
		else{
			decending = true;
			current = current -> next;
			next = current -> next;
		}
	}
	if (ascending && decending){
		return false;
	}
	return true;

}

