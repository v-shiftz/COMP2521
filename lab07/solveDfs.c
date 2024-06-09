// DFS maze solver

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Stack.h"

bool solve(Maze m) {
    Stack s = StackNew();
    int height = MazeHeight(m);
    int width = MazeWidth(m);
    bool **visited = createBoolMatrix(height, width);

    struct cell **predecessor = createCellMatrix(height, width);
    struct cell start = MazeGetStart(m);
    StackPush(s, start);
    visited[start.row][start.col] = true;
    predecessor[start.row][start.col] = (struct cell){-1, -1};
    while (!StackIsEmpty(s)) {
        struct cell current = StackPop(s);

        if (MazeVisit(m, current)) {
            struct cell curr = current;
            while (!(curr.row == start.row && curr.col == start.col)) {
                MazeMarkPath(m, curr);
                curr = predecessor[curr.row][curr.col];
            }
            MazeMarkPath(m, start);
            StackFree(s);
            freeBoolMatrix(visited);
            freeCellMatrix(predecessor);
            return true;
        }

        struct cell directions[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 4; i++) {
            struct cell next = {current.row + directions[i].row, current.col + directions[i].col};

            if (next.row >= 0 && next.row < height && next.col >= 0 && next.col < width && !visited[next.row][next.col] && !MazeIsWall(m, next)) {
                visited[next.row][next.col] = true;
                predecessor[next.row][next.col] = current;
                StackPush(s, next);
            }
        }
    }

    StackFree(s);
    freeBoolMatrix(visited);
    freeCellMatrix(predecessor);
    return false;
}

