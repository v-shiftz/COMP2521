// Implementation of boulder climbing algorithms
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "climber.h"
#include "Wall.h"
#include "Queue.h"
#include "matrix.h"
#include "Graph.h"
#include "Pq.h"

// Main function for Task 2
struct path findShortestPath(Wall w, int reach, Colour colour) {
    int height = WallHeight(w);
    int width = WallWidth(w);
    Queue q = QueueNew();
    // Create a visited matrix to track visited positions 
    bool **visited = createBoolMatrix(height, width);
    // Create a predecessor matrix to track path
    struct cell **predecessor = malloc(height * sizeof(struct cell *));
    for (int i = 0; i < height; i++) {
        predecessor[i] = malloc(width * sizeof(struct cell));
        for (int j = 0; j < width; j++) {
            struct cell initialize = {-1, -1};
            predecessor[i][j] = initialize;
        }
    }
    // Enqueue potential starting rocks
    for (int y = 0; y <= reach; y++) {
        for (int x = 0; x < width; x++) {
            if (WallGetRockColour(w, y, x) == colour) {       
                struct cell startCell = {y, x};
                QueueEnqueue(q, startCell);
                visited[y][x] = true;
                // Use -2 to indicate start cell
                struct cell start = {-2, -2};
                predecessor[y][x] = start;
            }
        }
    }
    struct cell endCell;
    bool found = false;
    struct cell current;
    while (!QueueIsEmpty(q) && !found) {
            current = QueueDequeue(q);
        // Check if current position is within reach of the top
        if (height - current.row <= reach) {
            endCell = current;
            found = true;
            break;
        }
        // Get neighbors within reach
        int rocks = WallNumRocks(w);
        struct rock *neighbors = calloc(rocks, sizeof(struct rock));
        int count = WallGetColouredRocksInRange(w, current.row, current.col,
                                                 reach, colour, neighbors);
        for (int i = 0; i < count; i++) {
            int nr = neighbors[i].row;
            int nc = neighbors[i].col;
            if (!visited[nr][nc] && WallGetRockColour(w, nr, nc) == colour) {
                struct cell nextCell = {nr, nc};
                visited[nr][nc] = true;
                predecessor[nr][nc] = current;
                QueueEnqueue(q, nextCell);
            }
        }
        free (neighbors);
    }
    // Build the output path structure
    struct path result;
    if (found) {
        // Calculate the path by backtracking from endCell
        struct cell trace = endCell;
        int pathLength = 0;
        // Trace back until we hit the start cell
        while (predecessor[trace.row][trace.col].row != -2) { 
            trace = predecessor[trace.row][trace.col];
            pathLength++;
        }
        result.numRocks = pathLength + 1;
        result.rocks = malloc(result.numRocks * sizeof(struct rock));
        // Fill the path from end to start
        trace = endCell;
        for (int i = result.numRocks - 1; i >= 0; i--) {
            result.rocks[i].row = trace.row;
            result.rocks[i].col = trace.col;
            result.rocks[i].colour = colour;
            trace = predecessor[trace.row][trace.col];
        }
    } else {
        result.rocks = NULL;
        result.numRocks = 0;
    }
    freeBoolMatrix(visited);
    for (int i = 0; i < height; i++) {
        free(predecessor[i]);
    }
    free(predecessor);
    QueueFree(q);
    return result;
}

// Helper function for findMinEnergyPath
double calculateCost(struct rock a, struct rock b) {
    return sqrt(pow(a.col - b.col, 2) + pow(a.row - b.row, 2));
}

// Main Function for Task 3
struct path findMinEnergyPath(Wall w, int reach, int energyCosts[NUM_COLOURS]) {
    int height = WallHeight(w);
    int width = WallWidth(w);
    if (height == 0 || width == 0) {
        return (struct path){0, NULL};
    }
    int totalrocks = WallNumRocks(w);
    Graph g = GraphNew(totalrocks + 1);
    struct rock *array = malloc(totalrocks * sizeof(struct rock));
    WallGetAllRocks(w, array);
    for (int i = 0; i < totalrocks; i++) {
        for (int j = i + 1; j < totalrocks; j++) {
            double distance = calculateCost(array[i], array[j]);
            struct edge rocktorock = {i, j, energyCosts[array[j].colour]};
            if (distance <= reach) {
                GraphInsertEdge(g, rocktorock);
            }
        }
        if (array[i].row <= reach) {
            struct edge startToRock = {totalrocks, i , 0.5};
            GraphInsertEdge(g, startToRock);
        }
    }

    // Dijkstra's algorithm initialization
    int *dist = malloc((totalrocks + 1) * sizeof(int));
    bool *visited = malloc((totalrocks + 1) * sizeof(bool));
    int *prev = malloc((totalrocks + 1) * sizeof(int));
    for (int i = 0; i <= totalrocks; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[totalrocks] = 0;

    Pq pq = PqNew();
    for (int i = 0; i < totalrocks; i++) {
        if (array[i].row <= reach) {
            struct edge insert = {totalrocks, i, energyCosts[array[i].colour]};
            PqInsert(pq, insert);
            dist[i] = energyCosts[array[i].colour];
        }
    }
    // Dijkstra's Algorithm using Priority Queue
    while (!PqIsEmpty(pq)) {
        struct edge minEdge = PqExtract(pq);
        int u = minEdge.w;
        // Skip already visited nodes
        if (visited[u]) continue;
        visited[u] = true;

        // Iterate over all vertices to check for edges from u
        for (int v = 0; v <= totalrocks; v++) {
            if (v == u) continue;
            double weight = GraphIsAdjacent(g, u, v);
            if (weight > 0 && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                struct edge insert = {u, v, dist[v]};
                PqInsert(pq, insert);
            }
        }
    }
    // Find the endpoint with minimum distance on the top row
    int minDist = INT_MAX, endIndex = -1;
    for (int i = 0; i < totalrocks; i++) {
        if (array[i].row >= (height - reach) && dist[i] < minDist) {
            endIndex = i;
            minDist = dist[i];
        }
    }
    // Construct the path
    struct path p = {0, NULL};
    if (endIndex != -1) {
        int pathSize = 0, current = endIndex;
        while (current != -1) {
            pathSize++;
            current = prev[current];
        }
        p.rocks = malloc(pathSize * sizeof(struct rock));
        p.numRocks = pathSize;
        current = endIndex;
        for (int i = pathSize - 1; i >= 0; i--) {
            p.rocks[i] = array[current];
            current = prev[current];
        }
    }
    free(array);
    free(dist);
    free(visited);
    free(prev);
    PqFree(pq);
    GraphFree(g);
    return p;
}

// Helper struct for findMinTurnsPath
struct state {
    int rockIndex;
    int energy;
    int turns;
    int prevIndex;
};

// Main function for Task 4
struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    int numRocks = WallNumRocks(w);
    struct rock *rocks = malloc(numRocks * sizeof(struct rock));
    WallGetAllRocks(w, rocks);

    struct path result;
    result.numRocks = 0;
    result.rocks = NULL;
    int minTurns = INT_MAX;
    int height = WallHeight(w);

    // Check if the wall can be reached in one turn from the ground (Edge Case)
    struct rock bestRock;
    int foundDirectReach = 0;
    int lowestEnergyCost = maxEnergy + 1;
    for (int i = 0; i < numRocks; i++) {
        if (rocks[i].row <= reach && rocks[i].row >= height - reach) {
            int energyCost = energyCosts[rocks[i].colour];
            if (energyCost <= maxEnergy && energyCost < lowestEnergyCost) {
                bestRock = rocks[i];
                lowestEnergyCost = energyCost;
                foundDirectReach = 1;
            }
        }
    }
    if (foundDirectReach) {
        // If directly reachable, allocate path and set result
        result.rocks = malloc(sizeof(struct rock));
        result.rocks[0] = bestRock;
        result.numRocks = 1;
        free(rocks);
        return result;
    }

    struct state *queue = malloc(numRocks * numRocks * sizeof(struct state));
    int front = 0, back = 0;
    int **visited = malloc(numRocks * sizeof(int *));
    for (int i = 0; i < numRocks; i++) {
        visited[i] = calloc(maxEnergy + 1, sizeof(int));
    }
    // Initialize BFS with rocks reachable from the ground
    for (int i = 0; i < numRocks; i++) {
        if (rocks[i].row <= reach) {
            int remainingEnergy = maxEnergy - energyCosts[rocks[i].colour];
            if (remainingEnergy >= 0) {
                
                queue[back++] = (struct state){i, remainingEnergy, 1, -1};
                visited[i][remainingEnergy] = 1;
            }
        }
    }
    // Process the BFS queue
    while (front < back) {
        struct state current = queue[front++];
        if (rocks[current.rockIndex].row >= height - reach) {
            if (current.turns < minTurns) {
                minTurns = current.turns;
                int count = 0;
                int pathIndex = front - 1;
                while (pathIndex != -1) {
                    count++;
                    pathIndex = queue[pathIndex].prevIndex;
                }
                result.rocks = malloc(count * sizeof(struct rock));
                result.numRocks = count;
                pathIndex = front - 1;
                for (int j = count - 1; j >= 0; j--) {
                    result.rocks[j] = rocks[queue[pathIndex].rockIndex];
                    pathIndex = queue[pathIndex].prevIndex;
                }
            }
            continue;
        }
        // Try moving to all other reachable rocks or rest
        for (int i = 0; i < numRocks; i++) {
            if (i == current.rockIndex) continue;
            if (abs(rocks[i].row - rocks[current.rockIndex].row) <= reach &&
                abs(rocks[i].col - rocks[current.rockIndex].col) <= reach) {
                int energyAfterMove = current.energy - 
                                        energyCosts[rocks[i].colour];
                if (energyAfterMove >= 0 && !visited[i][energyAfterMove]) {
                    visited[i][energyAfterMove] = 1;
                    struct state insert = {i, energyAfterMove, 
                                            current.turns + 1, front - 1};
                    queue[back++] = insert;
                }
            }
        }
        // Check for rest condition
        if (current.energy < maxEnergy && !visited[current.rockIndex][maxEnergy]) {
            visited[current.rockIndex][maxEnergy] = 1;
            struct state insert = {current.rockIndex, maxEnergy, 
                                    current.turns + 1, front - 1};
            queue[back++] = insert;
        }
    }

    for (int i = 0; i < numRocks; i++) {
        free(visited[i]);
    }
    free(visited);
    free(queue);
    free(rocks);

    return result;
}

