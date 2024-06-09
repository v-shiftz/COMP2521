// Implementation of the Wall ADT

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Wall.h"

struct wall {
    int height;
    int width;
    int **rock_status;
    int numRocks;
};

static int compareRocks(const void *ptr1, const void *ptr2);

/**
 * Creates a new blank wall with the given dimensions
 */
Wall WallNew(int height, int width) {
    Wall newWall = malloc(sizeof(*newWall));

    newWall->height = height;
    newWall->width = width;
    newWall->numRocks = 0;
    newWall->rock_status = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        newWall->rock_status[i] = calloc(width, sizeof(int));
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            newWall->rock_status[i][j] = NONE;
        }
    }

    return newWall;
}

/**
 * Frees all memory allocated to the wall 
 */
void WallFree(Wall w) {
    for (int i = 0; i < w->height; i++) {
        free(w->rock_status[i]);
    }
    free(w->rock_status);
    free(w);
}

/**
 * Returns the height of the wall
 */
int WallHeight(Wall w) {
    return w->height;
}

/**
 * Returns the width of the wall
 */
int WallWidth(Wall w) {
    return w->width;
}

/**
 * Adds a rock to the wall
 * If there is already a rock at the given coordinates, replaces it
 */
void WallAddRock(Wall w, struct rock rock) {
    if (rock.row >= 0 && rock.row < w->height && rock.col >= 0 && rock.col < w->width) {
        if (w->rock_status[rock.row][rock.col] == NONE) {  // NONE is -1, indicating no rock.
            w->numRocks++;
        }
        w->rock_status[rock.row][rock.col] = rock.colour;
    }
}

/**
 * Returns the number of rocks on the wall
 */
int WallNumRocks(Wall w) {
    return w->numRocks;
}

/**
 * Returns the colour of the rock at the given coordinates, or NONE if
 * there is no rock at those coordinates.
 */
Colour WallGetRockColour(Wall w, int row, int col) {
    if (row < 0 || row >= w->height || col < 0 || col >= w->width) {
        return NONE;
    }
    return w->rock_status[row][col];
}

/**
 * Stores all rocks on the wall in the given `rocks` array and returns
 * the number of rocks stored. Assumes that the array is at least as
 * large as the number of rocks on the wall.
 */
int WallGetAllRocks(Wall w, struct rock rocks[]) {
   int count = 0;
    for (int i = 0; i < w->height; i++) {
        for (int j = 0; j < w->width; j++) {
            if (w->rock_status[i][j] != NONE) {
                rocks[count].row = i;
                rocks[count].col = j;
                rocks[count].colour = w->rock_status[i][j];
                count++;
            }
        }
    }
    return count;
}

/**
 * Stores all rocks that are within a distance of `dist` from the given
 * coordinates in the given `rocks` array and returns the number of rocks
 * stored. Assumes that the array is at least as large as the number of
 * rocks on the wall.
 */
int WallGetRocksInRange(Wall w, int row, int col, int dist,
                        struct rock rocks[]) {
    int count = 0;
    for (int i = 0; i < w->height; i++) {
        for (int j = 0; j < w->width; j++) {
            if (w->rock_status[i][j] != NONE) {  // Check if there's a rock at this position
                double dx = j - col;
                double dy = i - row;
                double distance = sqrt((dx * dx) + (dy * dy));
                if (distance <= dist) {
                    rocks[count].row = i;
                    rocks[count].col = j;
                    rocks[count].colour = w->rock_status[i][j];
                    count++;
                }
            }
        }
    }
    return count;
}

/**
 * Stores all rocks with the colour `colour` that are within a distance
 * of `dist` from the given coordinates in the given `rocks` array and
 * returns the number of rocks stored. Assumes that the array is at
 * least as large as the number of rocks on the wall.
 */
int WallGetColouredRocksInRange(Wall w, int row, int col, int dist,
                                Colour colour, struct rock rocks[]) {
    int count = 0;
    for (int i = 0; i < w->height; i++) {
        for (int j = 0; j < w->width; j++) {
            if (w->rock_status[i][j] != NONE) {
                double dx = j - col;
                double dy = i - row;
                double distance = sqrt(dx * dx + dy * dy);
                if (distance <= dist) {
                    rocks[count].row = i;
                    rocks[count].col = j;
                    rocks[count].colour = w->rock_status[i][j];
                    count++;
                }
            }
        }
    }
    return count;
}

////////////////////////////////////////////////////////////////////////

/**
 * Prints the wall out in a nice format
 * NOTE: DO NOT MODIFY THIS FUNCTION! This function will work once
 *       WallGetAllRocks and all the functions above it work.
 */
void WallPrint(Wall w) {
    int height = WallHeight(w);
    int width = WallWidth(w);
    int numRocks = WallNumRocks(w);
    struct rock *rocks = malloc(numRocks * sizeof(struct rock));
    WallGetAllRocks(w, rocks);
    qsort(rocks, numRocks, sizeof(struct rock), compareRocks);

    int i = 0;
    for (int y = height; y >= 0; y--) {
        for (int x = 0; x <= width; x++) {
            if ((y == 0 || y == height) && (x == 0 || x % 5 == 0)) {
                printf("+ ");
            } else if ((x == 0 || x == width) && (y == 0 || y % 5 == 0)) {
                printf("+ ");
            } else if (y == 0 || y == height) {
                printf("- ");
            } else if (x == 0 || x == width) {
                printf("| ");
            } else if (i < numRocks && y == rocks[i].row && x == rocks[i].col) {
                char *color;
                switch (rocks[i].colour) {
                    case GREEN: color = "\x1B[32m"; break;
                    case TEAL:  color = "\x1B[96m"; break;
                    case PINK:  color = "\x1B[95m"; break;
                    case RED:   color = "\x1B[91m"; break;
                    default:    color = "\x1B[0m";  break;
                }
                printf("%so %s", color, RESET);
                i++;
            } else {
                printf("\u00B7 ");
            }
        }
        printf("\n");
    }

    free(rocks);
}

static int compareRocks(const void *ptr1, const void *ptr2) {
    struct rock *r1 = (struct rock *)ptr1;
    struct rock *r2 = (struct rock *)ptr2;
    if (r1->row != r2->row) {
        return r2->row - r1->row;
    } else {
        return r1->col - r2->col;
    }
}

