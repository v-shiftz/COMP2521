// BFS maze solver

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Queue.h"

void enqueueIfValid(Queue q, Maze m, bool **visited, struct cell **predecessor, struct cell current, int nextRow, int nextCol){
    struct cell nextCell = {nextRow,nextCol};
    if(nextRow >= 0 && nextRow < MazeHeight(m) && nextCol >= 0 && nextCol < MazeHeight(m) && nextCol < MazeWidth(m) && (!visited[nextRow][nextCol]) && !MazeIsWall(m, nextCell)){
        visited[nextRow][nextCol] = true;
        predecessor[nextRow][nextCol] = current;
        QueueEnqueue(q, nextCell);
    }
}

bool solve(Maze m) {
    int height = MazeHeight(m);
    int width = MazeWidth(m);

    struct cell start = MazeGetStart(m);

    bool **visited = createBoolMatrix(height, width);

    struct cell **predecessor = createCellMatrix(height, width);

    Queue q = QueueNew();
    QueueEnqueue(q, start);

    bool exitFound = false;
    struct cell current;
    struct cell exitcell;
    visited[start.row][start.col] = true;
    while (!QueueIsEmpty(q) && !exitFound){
        current = QueueDequeue(q);
        if (MazeVisit(m, current)){
            exitFound = true;
            exitcell = current;
            break;
        }
        int newRow;
        int newCol;
        newRow = current.row + -1;
        newCol = current.col + 0;
        enqueueIfValid(q, m, visited, predecessor, current, newRow, newCol);
        newRow = current.row + 1;
        newCol = current.col + 0;
        enqueueIfValid(q, m, visited, predecessor, current, newRow, newCol);
        newRow = current.row + 0;
        newCol = current.col + -1;
        enqueueIfValid(q, m, visited, predecessor, current, newRow, newCol);
        newRow = current.row + 0;
        newCol = current.col + 1;
        enqueueIfValid(q, m, visited, predecessor, current, newRow, newCol);
    }

    if (exitFound) {
        struct cell curr = exitcell;
        while (!(curr.row == start.row && curr.col == start.col)) {
            MazeMarkPath(m, curr);
            curr = predecessor[curr.row][curr.col];
        }
        MazeMarkPath(m, start);

    }

    QueueFree(q);
    freeBoolMatrix(visited);
    freeCellMatrix(predecessor);
    return exitFound;
}
