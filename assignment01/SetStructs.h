// COMP2521 - Assignment 1

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct set {
	struct node *tree;  // DO NOT MODIFY/REMOVE THIS FIELD
	int size;
	// You may add more fields here if needed
};

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct node {
	int item;           // DO NOT MODIFY/REMOVE THIS FIELD
	struct node *left;  // DO NOT MODIFY/REMOVE THIS FIELD
	struct node *right; // DO NOT MODIFY/REMOVE THIS FIELD
	int height;
	int size;
	// You may add more fields here if needed
};
// You may define more structs here if needed 

////////////////////////////////////////////////////////////////////////
// Cursors

struct cursor {
	// You may add more fields here if needed
};

////////////////////////////////////////////////////////////////////////

