// Implementation of the Set ADT
// COMP2521 - Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Set.h"
#include "SetStructs.h"

//part 1 and 3
struct node *bstInsert(struct node *t, int item, int *inserted);
static struct node *newNode(int item);
int height(struct node *t);
int max(int a, int b);
int getBalance(struct node *t);
struct node *avlBalance(struct node *t);
struct node *rotateRight(struct node *root);
struct node *rotateLeft(struct node *root);
struct node *bstDelete(struct node *tree, int item, int *deleted);
struct node *bstJoin(struct node *t1, struct node *t2);
int SetSize(Set s);
bool bstFind(struct node *t, int val);

//part 2
void bstInOrder(struct node *t, FILE *out, int *isFirst);
static void mergeSet(struct node *t, Set resultSet);
static void IntersectSet(struct node *t, Set s2, Set resultSet);
bool checkSet(struct node *t, Set s2);

//part 4
int getSize(struct node *t);
int getNodeAtIndex(struct node *t, int k);
int getIndexAtNode(struct node *t, int elem);
////////////////////////////////////////////////////////////////////////
// Basic Set Operations

/**
 * Creates a new empty set
 */
Set SetNew(void) {
	struct set* newSet = malloc(sizeof(struct set));
	if (newSet == NULL){
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
   	newSet->tree = NULL;
	newSet->size = 0;
    return newSet;
}

/**
 * Frees all memory allocated to the set 
 */
static void freeNodes(struct node *node) {
    if (node == NULL) {
        return;
    }
    freeNodes(node->left);
    freeNodes(node->right);
    free(node);
}

void SetFree(Set s) {
	if (s != NULL){
		freeNodes(s->tree);
		free(s);
	}
}

/**
 * Inserts an item into the set
 */
void SetInsert(Set s, int item) {
	if (s == NULL){
		return;
	}
	int inserted = 0;
	s->tree = bstInsert(s->tree, item, &inserted);
	if (inserted){
		s->size++;
	}
}

// Helper function for SetInsert
struct node *bstInsert(struct node *t, int item, int *inserted){
	if (t == NULL){ //Base Case
		*inserted = 1;
		return newNode(item);
	}else if (item < t->item) {
		t->left = bstInsert(t->left, item, inserted);
	} else if (item > t->item) {
		t->right = bstInsert(t->right, item, inserted);
	}else{
		*inserted = 0;
		return t;
	}
	return avlBalance(t);
}

// Helper function to create new node for bstInsert
static struct node *newNode(int item) {
	struct node *new = malloc(sizeof(*new));
	if (new == NULL) { //Base Case
		return NULL;
	}
	new->item = item;
	new->left = NULL;
	new->right = NULL;
	new->height = 1;
	new->size = 1;
	return new;
}

// Calculate Height
int height(struct node *t){
	if (t == NULL){
		return -1;
	}
	return t->height;
}

int max(int a, int b){
	return (a > b) ? a : b;
}

int getBalance(struct node *t){
	if (t == NULL){
		return 0;
	}
	return height(t -> left) - height (t -> right);
}

struct node *avlBalance(struct node *t){
	int bal = getBalance(t);
	if (bal > 1){
		if (getBalance(t -> left ) < 0){
			t ->left = rotateLeft(t -> left);
		}
		t = rotateRight(t);
	}else if (bal < -1){
		if (getBalance(t -> right) > 0){
			t -> right = rotateRight(t -> right);
		}
		t = rotateLeft(t);
	}
	return t;
}

// Helper function referenced from lecture slides
struct node *rotateRight(struct node *root) {
	if (root == NULL || root->left == NULL) return root;
	struct node *newRoot = root->left;
	root->left = newRoot->right;
	newRoot->right = root;

	newRoot->height = max(height(newRoot->left), height(newRoot->right))+ 1;
	newRoot->size = getSize(newRoot->left) + getSize(newRoot->right) + 1;
	root->height = max(height(root -> left), height(root -> right)) + 1;
	root->size = getSize(root->left) + getSize(root->right) + 1;
	return newRoot;
}

// Helper function eferenced from lecture slides
struct node *rotateLeft(struct node *root) {
	if (root == NULL || root->right == NULL) return root;
	struct node *newRoot = root->right;
	root->right = newRoot->left;
	newRoot->left = root;

	newRoot->height = max(height(newRoot->left), height(newRoot->right))+ 1;
	newRoot->size = getSize(newRoot->left) + getSize(newRoot->right) + 1;
	root->height = max(height(root -> left), height(root -> right)) + 1;
	root->size = getSize(root->left) + getSize(root->right) + 1;
	return newRoot;
}

/**
 * Deletes an item from the set
 */
void SetDelete(Set s, int item) {
	if (s == NULL || s->tree == NULL){
		return;
	}
	int deleted = 0;
	s->tree = bstDelete(s->tree, item, &deleted);
	if (deleted){
		s->size--;
	}
}

// Helper function for SetDelete referenced from lecture slides
struct node *bstDelete(struct node *tree, int item, int *deleted){
	if (tree == NULL) { //Base Case
		return tree;
	} else if (item < tree->item) {
		tree->left = bstDelete(tree->left, item, deleted);
	} else if (item > tree->item) {
		tree->right = bstDelete(tree->right, item, deleted);
	} else {
		if (tree->left == NULL && tree->right == NULL) {
			free(tree);
			*deleted = 1;
			return NULL;
		} else if (tree->right == NULL) {
			struct node *tmp = tree->left;
			free(tree);
			*deleted = 1;
			return tmp;
		} else if (tree->left == NULL) {
			struct node *tmp = tree->right;
			free(tree);
			*deleted = 1;
			return tmp;
		} else {
			struct node *joined = bstJoin(tree->left, tree->right);
			*deleted = 1;
			free(tree);
			return joined;
		}
	}
	return avlBalance(tree);
}

// Helper function for bstDelete referenced from lecture slides
struct node *bstJoin(struct node *t1, struct node *t2) {
	if (t1 == NULL) {
		return t2;
	}
	if (t2 == NULL) {
		return t1;
	}
	struct node *parent = NULL;
	struct node *min = t2;
	while (min->left != NULL) {
		parent = min;
		min = min->left;
	}

	if (parent == NULL) {
		t2->left = t1;
		return t2;
	}

	parent->left = min->right;
	min->left = t1;
	min->right = t2;
	return min;
}

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s) {
	return s->size;
}
/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item) {
	if (s == NULL || s -> tree == NULL){
		return false;
	}
	bool found = false;
	found = bstFind(s->tree, item);
	return found;

}

// Helper function referenced from lecture slides
bool bstFind(struct node *t, int val) {
	if (t == NULL) {
		return false;
	} else if (val < t->item) {
		return bstFind(t->left, val);
	} else if (val > t->item) {
		return bstFind(t->right, val);
	} else { // (val == t->value)
		return true;
	}
}
/**
 * Prints the elements in the set to the given file in ascending order
 * between curly braces, with items separated by a comma and space
 */

void SetPrint(Set s, FILE *out) {
	if (s == NULL || out == NULL) return;
	fprintf(out, "{");

	int isFirst = 1;
	bstInOrder(s->tree, out, &isFirst);

	fprintf(out, "}");
}

// Helper function to print the tree out in order
void bstInOrder(struct node *t, FILE *out, int *isFirst) {
	if (t == NULL) return;

	bstInOrder(t->left, out, isFirst);
	if (*isFirst){
		fprintf(out, "%d", t->item);
		*isFirst = 0;
	}else{
		fprintf(out, ", %d", t->item);
	}
	bstInOrder(t->right, out, isFirst);
}
////////////////////////////////////////////////////////////////////////
// Common Set Operations

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2) {
	Set resultSet = SetNew();

	if (s1 != NULL || s1->tree != NULL){
		mergeSet(s1 -> tree, resultSet);
	}

	if (s2 != NULL || s2->tree != NULL){
		mergeSet(s2 -> tree, resultSet);
	}
	return resultSet;
}

// Helper function that adds all elements in using the basis of bstInOrder
static void mergeSet(struct node *t, Set resultSet){
	if (t == NULL){
		return;
	}
	mergeSet(t -> left, resultSet);
	SetInsert(resultSet, t -> item);
	mergeSet(t -> right, resultSet);
}
/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2) {
	Set resultSet = SetNew();

	if (s1 == NULL || s1->tree == NULL || s2 == NULL || s2->tree == NULL){
		return NULL;
	}
	IntersectSet(s1 -> tree, s2, resultSet);
	return resultSet;

}

// Helper function to only add elements that are present in both sets
static void IntersectSet(struct node *t, Set s2, Set resultSet){
	if (t == NULL){
		return;
	}
	IntersectSet(t -> left, s2, resultSet);
	if(SetContains(s2, t ->item)){
		SetInsert(resultSet, t -> item);
	}
	IntersectSet(t -> right, s2, resultSet);
}

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2) {
	if (s1 -> size != s2 -> size){
		return false;
	}
	return checkSet(s1 -> tree, s2);
}

// Helper set to check if both sets are equal
bool checkSet(struct node *t, Set s2){
	if (t == NULL){
		return true;
	}

	if (!checkSet(t -> left, s2)){
		return false;
	}
	if (!SetContains(s2, t -> item)){
		return false;
	}
	return checkSet(t -> right, s2);
}
/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2) {
	if (s1 == NULL){
		return true;
	}
	if (s2 == NULL){
		return true;
	}
	if (s1 -> size < s2 -> size){
		return checkSet(s1 -> tree, s2);
	}
	if  (s1 -> size > s2 ->size){
		return checkSet(s2 -> tree, s1);
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index, or UNDEFINED if the given
 * index is outside the range [0, n - 1] where n is the size of the set
 */
int SetAtIndex(Set s, int index) {
	if (s == NULL || index < 0 || index >= s -> size){
		return UNDEFINED;
	}
	return getNodeAtIndex(s->tree, index + 1);
}

int getSize(struct node *t){
	if (t == NULL){
		return 0;
	}
	return t -> size;
}

//Referenced from slide 3 of https://www.cs.cornell.edu/courses/cs211/2004su/slides/Topic20b.pdf
int getNodeAtIndex(struct node *t, int k){
	if (t == NULL){
		return UNDEFINED;
	}
	if (k == getSize(t -> left) + 1){
		return t -> item;
	}else if (k < getSize(t -> left) + 1){
		return getNodeAtIndex(t -> left, k);
	}else if (k > getSize(t -> left) + 1){
		return getNodeAtIndex(t -> right, k - getSize(t -> left) - 1);
	}else{
		return t -> item;
	}
}

/**
 * Returns the index of the given element in the set if it exists, and
 * -1 otherwise
 */
int SetIndexOf(Set s, int elem) {
	if (s == NULL || !SetContains(s, elem)){
		return -1;
	}
	return getIndexAtNode(s -> tree, elem);
}

// Helper function for SetIndexOf to return the correct index
int getIndexAtNode(struct node *t, int elem){
	if (t == NULL){
		return -1;
	}
	if (elem < t -> item){
		return getIndexAtNode(t -> left, elem);
	}else if (elem > t -> item){
		return getIndexAtNode(t -> right, elem) + getSize(t->left) + 1;
	}else{
		return getSize(t->left);
	}
}
////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the *start* of the set
 * (see the spec for details)
 */
SetCursor SetCursorNew(Set s) {
	// TODO
	return NULL;
}

/**
 * Frees all memory allocated to the given cursor
 */
void SetCursorFree(SetCursor cur) {
	// TODO
}

/**
 * Returns the element at the cursor's position, or UNDEFINED if the
 * cursor is positioned at the *start* or *end* of the set
 */
int SetCursorGet(SetCursor cur) {
	// TODO
	return UNDEFINED;
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * set if there is no next greatest element. Does not move the cursor if
 * it is already at the end. Returns false if the cursor is at the end
 * after this operation, and true otherwise.
 */
bool SetCursorNext(SetCursor cur) {
	// TODO
	return false;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * set if there is no next smallest element. Does not move the cursor if
 * it is already at the start. Returns false if the cursor is at the
 * start after this operation, and true otherwise.
 */
bool SetCursorPrev(SetCursor cur) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////

