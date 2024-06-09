// Interface to the Set ADT
// COMP2521 - Assignment 1

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef SET_H
#define SET_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define UNDEFINED INT_MIN

typedef struct set *Set;

////////////////////////////////////////////////////////////////////////
// Basic Set Operations

/**
 * Creates a new empty set
 */
Set SetNew(void);

/**
 * Frees all memory allocated to the set
 */
void SetFree(Set s);

/**
 * Inserts an item into the set
 */
void SetInsert(Set s, int item);

/**
 * Deletes an item from the set
 */
void SetDelete(Set s, int item);

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s);

/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item);

/**
 * Prints the elements in the set to the given file in ascending order
 * between curly braces, with items separated by a comma and space
 */
void SetPrint(Set s, FILE *out);

////////////////////////////////////////////////////////////////////////
// Common Set Operations

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2);

/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2);

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2);

/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2);

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index, or UNDEFINED if the given
 * index is outside the range [0, n - 1] where n is the size of the set
 */
int SetAtIndex(Set s, int index);

/**
 * Returns the index of the given element in the set if it exists, and
 * -1 otherwise
 */
int SetIndexOf(Set s, int elem);

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *SetCursor;

/**
 * Creates a new cursor positioned at the *start* of the set
 * (see the spec for details)
 */
SetCursor SetCursorNew(Set s);

/**
 * Frees all memory allocated to the given cursor
 */
void SetCursorFree(SetCursor cur);

/**
 * Returns the element at the cursor's position, or UNDEFINED if the
 * cursor is positioned at the *start* or *end* of the set
 */
int SetCursorGet(SetCursor cur);

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * set if there is no next greatest element. Does not move the cursor if
 * it is already at the end. Returns false if the cursor is at the end
 * after this operation, and true otherwise.
 */
bool SetCursorNext(SetCursor cur);

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * set if there is no next smallest element. Does not move the cursor if
 * it is already at the start. Returns false if the cursor is at the
 * start after this operation, and true otherwise.
 */
bool SetCursorPrev(SetCursor cur);

////////////////////////////////////////////////////////////////////////

#endif

