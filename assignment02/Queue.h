// Interface to the Queue ADT specialized for struct rock

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>

#include "Wall.h"
#include "cell.h"

typedef struct cell Item;

typedef struct queue *Queue;

/**
 * Creates a new empty queue
 * Time complexity: O(1)
 */
Queue QueueNew(void);

/**
 * Frees all memory allocated to the queue
 * Time complexity: O(n)
 */
void QueueFree(Queue q);

/**
 * Adds an item (rock) to the end of the queue
 * Time complexity: O(1)
 */
void QueueEnqueue(Queue q, Item item);

/**
 * Removes an item (rock) from the front of the queue and returns it
 * Assumes that the queue is not empty
 * Time complexity: O(1)
 */
Item QueueDequeue(Queue q);

/**
 * Gets the item (rock) at the front of the queue without removing it
 * Assumes that the queue is not empty
 * Time complexity: O(1)
 */
Item QueueFront(Queue q);

/**
 * Gets the size of the given queue
 * Time complexity: O(1)
 */
int QueueSize(Queue q);

/**
 * Returns true if the queue is empty, and false otherwise
 * Time complexity: O(1)
 */
bool QueueIsEmpty(Queue q);

/**
 * Prints the queue to the given file with items space-separated
 * Time complexity: O(n)
 */
 void QueueDump(Queue q, FILE *fp);

#endif
