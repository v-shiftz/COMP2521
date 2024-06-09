
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"

#define NO_WINNER 0

int findWinner(int *ballots, int numBallots);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "usage: %s <ballots>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int numBallots = argc - 1;
	int *ballots = malloc(numBallots * sizeof(int));
	for (int i = 1; i < argc; i++) {
		ballots[i - 1] = atoi(argv[i]);
	}

	int winner = findWinner(ballots, numBallots);
	if (winner == NO_WINNER) {
		printf("Tie!\n");
	} else {
		printf("The winner is candidate #%d\n", winner);
	}

	free(ballots);
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Given an array of ballots for candidates, where each ballot is represented
 * by the candidate number written on it, return the candidate number of the
 * winner. For example, given the votes [1, 2, 1, 1, 3, 3], the winner is
 * candidate #1.
 *
 * If there is a tie, return NO_WINNER (i.e. 0). Naturally, you can assume that
 * all candidate numbers are greater than 0. You can also assume that there is
 * at least one ballot.
 */
int findWinner(int *ballots, int numBallots) {
	HashTable table = HashTableNew();
    int maxVotes = 0;
    int winner = NO_WINNER;
    for (int i = 0; i < numBallots; i++) {
        int candidate = ballots[i];
        int votes;
        if (HashTableContains(table, candidate)) {
            votes = HashTableGet(table, candidate) + 1;
        } else {
            votes = 1;
        }
        HashTableInsert(table, candidate, votes);
        if (votes > maxVotes) {
            maxVotes = votes;
            winner = candidate;
        }
    }
    int candidatesWithMaxVotes = 0;
    for (int candidate = 1; candidate <= numBallots; candidate++) {
        if (HashTableContains(table, candidate) && HashTableGet(table, candidate) == maxVotes) {
            candidatesWithMaxVotes++;
            if (candidatesWithMaxVotes > 1) {
                winner = NO_WINNER;
                break;
            }
        }
    }

    HashTableFree(table);
    return winner;
}


