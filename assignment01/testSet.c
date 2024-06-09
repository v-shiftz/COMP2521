// COMP2521 - Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Set.h"
#include "SetStructs.h"

void testSetInsert(void);
void testSetDelete(void);
void testSetSize(void);
void testSetContains(void);
void testSetPrint(void);
void checkFileContents(FILE *file, char *str);

void testSetUnion(void);
void testSetIntersection(void);
void testSetEquals(void);
void testSetSubset(void);

void testBalance1(void);
void testBalance2(void);
bool isHeightBalanced(struct node *t);
bool doIsHeightBalanced(struct node *t, int *height);

void testSetAtIndex(void);
void testSetIndexOf(void);

void testSetCursor1(void);
void testSetCursor2(void);
// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct {
	asciinode *left, *right;
	// length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	// -1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	// max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
static void print_level(asciinode *node, int x, int level, FILE *out);
static void compute_edge_lengths(asciinode *node);
static void compute_lprofile(asciinode *node, int x, int y);
static void compute_rprofile(asciinode *node, int x, int y);
static asciinode *build_ascii_tree(struct node *t);
static void free_ascii_tree(asciinode *node);

#define MAX_HEIGHT 1000
static int lprofile[MAX_HEIGHT];
static int rprofile[MAX_HEIGHT];
#define INFINITY (1 << 20)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static int gap = 3; // gap between left and right nodes

// used for printing next node in the same level,
// this is the x coordinate of the next char printed
static int print_next;

// prints ascii tree for given Tree structure
static void doBstPrint(struct node *t, FILE *out)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL)
		return;
	proot = build_ascii_tree(t);
	compute_edge_lengths(proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN(xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level(proot, -xmin, i, out);
		fprintf(out, "\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		fprintf(out,
			"(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree(proot);
}

// This function prints the given level of the given tree, assuming
// that the node has the given x cordinate.
static void print_level(asciinode *node, int x, int level, FILE *out)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	if (level == 0) {
		for (i = 0;
			 i < (x - print_next - ((node->lablen - isleft) / 2));
			 i++)
			fprintf(out, " ");
		print_next += i;
		fprintf(out, "%s", node->label);
		print_next += node->lablen;
	} else if (node->edge_length >= level) {
		if (node->left != NULL) {
			for (i = 0; i < (x - print_next - (level)); i++) {
				fprintf(out, " ");
			}
			print_next += i;
			fprintf(out, "/");
			print_next++;
		}
		if (node->right != NULL) {
			for (i = 0; i < (x - print_next + (level)); i++) {
				fprintf(out, " ");
			}
			print_next += i;
			fprintf(out, "\\");
			print_next++;
		}
	} else {
		print_level(
			node->left,
			x - node->edge_length - 1,
			level - node->edge_length - 1,
			out);
		print_level(
			node->right,
			x + node->edge_length + 1,
			level - node->edge_length - 1,
			out);
	}
}

// This function fills in the edge_length and
// height fields of the specified tree
static void compute_edge_lengths(asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL)
		return;
	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else {
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
				lprofile[i] = INFINITY;
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		delta = 4;
		for (i = 0; i < hmin; i++) {
			int w = gap + 1 + rprofile[i] - lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		// If the node has two children of height 1, then we allow the
		// two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
			(node->right != NULL && node->right->height == 1)) &&
			delta > 4)
			delta--;
		node->edge_length = ((delta + 1) / 2) - 1;
	}

	// now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX(node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX(node->right->height + node->edge_length + 1, h);
	node->height = h;
}

static asciinode *build_ascii_tree_recursive(struct node *t)
{
	asciinode *node;

	if (t == NULL)
		return NULL;
	node = malloc(sizeof(asciinode));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);
	if (node->left != NULL)
		node->left->parent_dir = -1;
	if (node->right != NULL)
		node->right->parent_dir = 1;
	sprintf(node->label, "%d", t->item);
	node->lablen = (int) strlen(node->label);

	return node;
}

// Copy the tree into the ascii node structre
static asciinode *build_ascii_tree(struct node *t)
{
	asciinode *node;
	if (t == NULL)
		return NULL;
	node = build_ascii_tree_recursive(t);
	node->parent_dir = 0;
	return node;
}

// Free all the nodes of the given tree
static void free_ascii_tree(asciinode *node)
{
	if (node == NULL)
		return;
	free_ascii_tree(node->left);
	free_ascii_tree(node->right);
	free(node);
}

// The following function fills in the lprofile array for the given
// tree. It assumes that the center of the label of the root of this tree
// is located at a position(x,y).  It assumes that the edge_length
// fields have been computed for this tree.
static void compute_lprofile(asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
	if (node->left != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			lprofile[y + i] = MIN(lprofile[y + i], x - i);
	}
	compute_lprofile(
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_lprofile(
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}

static void compute_rprofile(asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL)
		return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
	if (node->right != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			rprofile[y + i] = MAX(rprofile[y + i], x + i);
	}
	compute_rprofile(
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_rprofile(
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}



void bstPrint(struct node *t, FILE *out) {
	doBstPrint(t, out);
}
void bstShow(struct node *t) {
	bstPrint(t, stdout);
}
int main(int argc, char *argv[]) {
	testSetInsert();
	testSetDelete();
	testSetSize();
	testSetContains();
	testSetPrint();

	testSetUnion();
	testSetIntersection();
	testSetEquals();
	testSetSubset();

	testBalance1();
	testBalance2();

	testSetAtIndex();
	testSetIndexOf();

	testSetCursor1();
	testSetCursor2();
}

void testSetInsert(void) {
	// NOTE: SetInsert can't be tested on its own unless we directly
	//       access the internal representation of the ADT

	Set s = SetNew();
	SetInsert(s, 5);
	assert(SetSize(s) == 1);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	SetInsert(s, 4);
	assert(SetSize(s) == 4);
	SetInsert(s, 5);
	assert(SetSize(s) == 4);

	SetFree(s);
}

void testSetDelete(void) {
	// NOTE: SetDelete can't be tested without SetInsert

	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	SetDelete(s, 2);
	assert(SetSize(s) == 3);
	SetDelete(s, 8);
	assert(SetSize(s) == 2);
	SetDelete(s, 3);
	assert(SetSize(s) == 2);

	SetFree(s);
}

void testSetSize(void) {
	Set s = SetNew();
	assert(SetSize(s) == 0);
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	assert(SetSize(s) == 4);
	SetInsert(s, 7);
	SetInsert(s, 2);
	assert(SetSize(s) == 5);

	SetFree(s);
}

void testSetContains(void) {
	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	assert(SetContains(s, 2));
	assert(SetContains(s, 4));
	assert(SetContains(s, 5));
	assert(SetContains(s, 8));
	assert(!SetContains(s, 3));

	SetFree(s);
}

void testSetPrint(void) {
	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	FILE *out = tmpfile();
	SetPrint(s, out);
	checkFileContents(out, "{2, 4, 5, 8}");
	fclose(out);

	SetFree(s);
}

/**
 * This function checks if the contents of a file matches the expected
 * contents. If the contents do not match, the program will exit with an
 * assertion error.
 */
void checkFileContents(FILE *file, char *expectedContents) {
	fflush(file);
	fseek(file, 0, SEEK_SET);
	char *line = NULL;
	size_t n = 0;
	getline(&line, &n, file);
	assert(strcmp(line, expectedContents) == 0);
	free(line);
}

////////////////////////////////////////////////////////////////////////

void testSetUnion(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 2);
	SetInsert(a, 8);
	SetInsert(a, 4);
	bstShow(a->tree);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 8);
	SetInsert(b, 7);
	SetInsert(b, 5);
	SetInsert(b, 2);
	bstShow(b->tree);
	
	Set c = SetUnion(a, b);
	assert(SetContains(c, 2));
	assert(SetContains(c, 3));
	assert(SetContains(c, 4));
	assert(SetContains(c, 5));
	assert(SetContains(c, 7));
	assert(SetContains(c, 8));
	assert(SetSize(c) == 6);
	bstShow(c->tree);
	SetFree(a);
	SetFree(b);
	SetFree(c);
}

void testSetIntersection(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 2);
	SetInsert(a, 8);
	SetInsert(a, 4);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 8);
	SetInsert(b, 7);
	SetInsert(b, 5);

	Set c = SetIntersection(a, b);
	assert(SetContains(c, 5));
	assert(SetContains(c, 8));
	assert(!SetContains(c, 2));
	assert(!SetContains(c, 3));
	assert(!SetContains(c, 4));
	assert(!SetContains(c, 7));
	assert(SetSize(c) == 2);

	SetFree(a);
	SetFree(b);
	SetFree(c);
}

void testSetEquals(void) {
	Set a = SetNew();
	SetInsert(a, 4);
	SetInsert(a, 2);
	SetInsert(a, 7);
	SetInsert(a, 1);

	Set b = SetNew();
	SetInsert(b, 2);
	SetInsert(b, 4);
	SetInsert(b, 1);
	SetInsert(b, 7);

	Set c = SetNew();
	SetInsert(c, 4);
	SetInsert(c, 2);
	SetInsert(c, 7);
	SetInsert(c, 3);

	assert(SetEquals(a, b));
	assert(!SetEquals(a, c));

	SetFree(a);
	SetFree(b);
	SetFree(c);
}

void testSetSubset(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 3);
	SetInsert(a, 6);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 9);
	SetInsert(b, 5);
	SetInsert(b, 6);

	Set c = SetNew();
	SetInsert(c, 4);
	SetInsert(c, 6);
	SetInsert(c, 3);
	SetInsert(c, 7);

	Set d = SetNew();
	SetInsert(d, 5);
	SetInsert(d, 3);
	SetInsert(d, 6);
	SetInsert(d, 7);

	Set e = SetNew();

	assert(SetSubset(a, b));
	assert(!SetSubset(a, c));
	assert(SetSubset(a, d));
	assert(SetSubset(a, e));
	assert(SetSubset(e, d));

	SetFree(a);
	SetFree(b);
	SetFree(c);
}

////////////////////////////////////////////////////////////////////////

void testBalance1(void) {
	Set s = SetNew();

	SetInsert(s, 8);
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 3);
	SetInsert(s, 4);
	SetInsert(s, 5);
	SetInsert(s, 6);
	SetInsert(s, 7);
	SetInsert(s, 8);
	SetInsert(s, 9);
	SetInsert(s, 10);
	SetInsert(s, 11);
	SetInsert(s, 12);
	SetInsert(s, 13);
	SetInsert(s, 14);
	SetInsert(s, 15);
	SetInsert(s, 16);
	SetInsert(s, 17);
	SetInsert(s, 18);
	SetInsert(s, 19);
	SetInsert(s, 20);
	SetInsert(s, 21);
	SetInsert(s, 22);
	SetInsert(s, 23);
	SetInsert(s, 24);
	SetInsert(s, 25);
	SetInsert(s, 26);
	SetInsert(s, 27);
	SetInsert(s, 28);
	SetInsert(s, 29);
	SetInsert(s, 30);
	SetInsert(s, 31);
	SetInsert(s, 12);
	SetInsert(s, 32);
	SetInsert(s, 33);
	SetInsert(s, 34);

	// The tree should have been rebalanced after inserting 2
	// NOTE: Normally, a user should not have access to the concrete
	//       representation of an ADT, but since we have #included
	//       SetStructs.h, we have access for testing purposes.
	assert(isHeightBalanced(s->tree));

	SetFree(s);
}

void testBalance2(void) {
	Set s = SetNew();

	SetInsert(s, 8);
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 3);
	SetInsert(s, 4);
	SetInsert(s, 5);
	SetInsert(s, 6);
	SetInsert(s, 7);
	SetInsert(s, 8);
	SetInsert(s, 9);
	SetInsert(s, 10);
	SetInsert(s, 11);
	SetDelete(s, 5);
	SetDelete(s, 4);
	SetDelete(s, 7);

	// The tree should have been rebalanced after deleting 7
	assert(isHeightBalanced(s->tree));

	SetFree(s);
}

/**
 * Checks if the given tree is height-balanced
 */
bool isHeightBalanced(struct node *t) {
	int height = -1;
	return doIsHeightBalanced(t, &height);
}

bool doIsHeightBalanced(struct node *t, int *height) {
	if (t == NULL) {
		*height = -1;
		return true;
	}

	int lHeight = -1;
	int rHeight = -1;
	if (doIsHeightBalanced(t->left, &lHeight)
			&& doIsHeightBalanced(t->right, &rHeight)
			&& abs(lHeight - rHeight) <= 1) {
		*height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		return true;
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////

void testSetAtIndex(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	//printf("%d\n", s->tree->size);
	SetInsert(s, 2);
	//printf("%d\n", s->tree->size);
	SetInsert(s, 8);
	//printf("%d\n", s->tree->size);
	SetInsert(s, 4);
	//printf("%d\n", s->tree->size);
	
	//bstShow(s->tree);
	//printf("%d\n", s->tree->size);
	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 4);
	assert(SetAtIndex(s, 2) == 5);
	assert(SetAtIndex(s, 3) == 8);

	SetInsert(s, 3);
	//bstShow(s->tree);
	//printf("%d\n", s->tree->size);

	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 3);
	assert(SetAtIndex(s, 2) == 4);
	assert(SetAtIndex(s, 3) == 5);
	assert(SetAtIndex(s, 4) == 8);

	SetDelete(s,3);
	//bstShow(s->tree);
	//printf("%d\n", s->tree->size);
	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 4);
	assert(SetAtIndex(s, 2) == 5);
	assert(SetAtIndex(s, 3) == 8);

	SetDelete(s, 8);
	//bstShow(s->tree);
	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 4);
	assert(SetAtIndex(s, 2) == 5);

	SetDelete(s, 5);
	//bstShow(s->tree);
	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 4);

	SetDelete(s, 4);
	//bstShow(s->tree);
	assert(SetAtIndex(s, 0) == 2);

	SetInsert(s,3);
	SetInsert(s,10);
	//bstShow(s->tree);
	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 3);
	assert(SetAtIndex(s, 2) == 10);
	SetFree(s);

}

void testSetIndexOf(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	//bstShow(s->tree);
	//printf("%d\n", SetIndexOf(s,2));
	//printf("%d\n", SetIndexOf(s,4));
	//printf("%d\n", SetIndexOf(s,5));
	//printf("%d\n", SetIndexOf(s,8));
	assert(SetIndexOf(s, 2) == 0);
	assert(SetIndexOf(s, 4) == 1);
	assert(SetIndexOf(s, 5) == 2);
	assert(SetIndexOf(s, 8) == 3);

	SetInsert(s, 3);
	//bstShow(s->tree);
	//printf("%d\n", SetIndexOf(s,2));
	//printf("%d\n", SetIndexOf(s,3));
	//printf("%d\n", SetIndexOf(s,4));
	//printf("%d\n", SetIndexOf(s,5));
	//printf("%d\n", SetIndexOf(s,8));
	assert(SetIndexOf(s, 2) == 0);
	assert(SetIndexOf(s, 3) == 1);
	assert(SetIndexOf(s, 4) == 2);
	assert(SetIndexOf(s, 5) == 3);
	assert(SetIndexOf(s, 8) == 4);

	SetDelete(s, 3);
	SetDelete(s, 4);
	SetDelete(s, 5);
	SetDelete(s, 8);
	SetDelete(s, 2);

	
	//bstShow(s->tree);
	SetInsert(s, 1);
	SetInsert(s, 99);
	//bstShow(s->tree);
	//printf("%d\n", SetIndexOf(s, 100));
	assert(SetIndexOf(s, 100)==-1);
	SetFree(s);
}

////////////////////////////////////////////////////////////////////////

void testSetCursor1(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	SetCursor cur = SetCursorNew(s);
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 4);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//                 ^
	assert(SetCursorGet(cur) == 8);

	assert(!SetCursorNext(cur));
	// start  2  4  5  8  end
	//                     ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//                 ^
	assert(SetCursorGet(cur) == 8);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 4);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	assert(!SetCursorPrev(cur));
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	SetCursorFree(cur);
	SetFree(s);
}

void testSetCursor2(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	SetCursor cur = SetCursorNew(s);
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	SetInsert(s, 3);
	// start  2  3  4  5  8  end
	//        ^

	assert(SetCursorNext(cur));
	// start  2  3  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 3);

	SetDelete(s, 4);
	// start  2  3  5  8  end
	//           ^

	assert(SetCursorNext(cur));
	// start  2  3  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	SetCursorFree(cur);
	SetFree(s);
}

