
#include "list.h"

static void appendValues(List l, int val){
	Node n = newNode(val);
	n -> next = l -> head;
	l -> head = n;
}

static bool listContains(List l, int val) {
	for (Node current = l -> head; current != NULL; current = current -> next) {
		if (current -> value == val) {
			return true;
		}
	}
	return false;
}

List listSetUnion(List s1, List s2) {
	List s3 = newList();
	for (struct node *current = s1 -> head; current != NULL; current = current -> next) {
		appendValues(s3, current -> value);
	}
	for (Node current = s2 -> head; current != NULL; current = current -> next) {
		if (!(listContains(s3, current -> value))) {
			appendValues(s3, current -> value);
		}
	}
	return s3;
}


