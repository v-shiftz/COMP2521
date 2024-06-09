struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    int numRocks = WallNumRocks(w);
    struct rock *rocks = malloc(numRocks * sizeof(struct rock));
    WallGetAllRocks(w, rocks);

    struct state *queue = malloc(100000 * sizeof(struct state)); // Arbitrary large number for BFS queue
    int front = 0, back = 0;

    // Visited states: 2D array where the first dimension is rock index and the second dimension is energy
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

    struct path result;
    result.numRocks = 0;
    result.rocks = NULL;
    int minTurns = INT_MAX;

    // Process the BFS queue
    while (front < back) {
        struct state current = queue[front++];

        // If we reach the top row
        if (rocks[current.rockIndex].row >= WallHeight(w) - reach) {
            if (current.turns < minTurns) {
                minTurns = current.turns;
                // Trace back to reconstruct the path
                int count = 0;
                struct state *temp = &current;
                while (temp->prevIndex != -1) {
                    count++;
                    temp = &queue[temp->prevIndex];
                }
                free(result.rocks);
                result.rocks = malloc(count * sizeof(struct rock));
                result.numRocks = count;
                temp = &current;
                for (int j = count - 1; j >= 0; j--) {
                    result.rocks[j] = rocks[temp->rockIndex];
                    temp = &queue[temp->prevIndex];
                }
            }
            continue;
        }

        // Try moving to all other reachable rocks
        for (int i = 0; i < numRocks; i++) {
            if (i == current.rockIndex) continue;
            if (abs(rocks[i].row - rocks[current.rockIndex].row) <= reach &&
                abs(rocks[i].col - rocks[current.rockIndex].col) <= reach) {
                int energyAfterMove = current.energy - energyCosts[rocks[i].colour];
                
                if (energyAfterMove < 0 && !visited[current.rockIndex][maxEnergy]) {
                    // Enforce rest when not enough energy
                    visited[current.rockIndex][maxEnergy] = 1;
                    queue[back++] = (struct state){current.rockIndex, maxEnergy, current.turns + 1, front - 1};

                } else if (energyAfterMove > 0 && !visited[i][energyAfterMove]) {
                    printf("energy: %d\n", energyAfterMove);
                    visited[i][energyAfterMove] = 1;
                    queue[back++] = (struct state){i, energyAfterMove, current.turns + 1, front - 1};
                }
            }
        }
        // Check for rest condition
    }

    for (int i = 0; i < numRocks; i++) {
        free(visited[i]);
    }
    free(visited);
    free(queue);
    free(rocks); // Don't forget to free the rocks array

    return result;
}