
#include "list.h"

static void addList(List l, int val) {
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

List listSetDifference(List l1, List l2) {
	List l3 = newList();
	for (Node current = l1 -> head; current != NULL; current = current -> next) {
		if (!(listContains(l2, current -> value))) {
			addList(l3, current -> value);
		}
	}
	return l3;
}

