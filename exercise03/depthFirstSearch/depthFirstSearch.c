
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
static void depthFirstSearchRec(Graph g, int src, bool *visited) {
	printf("%d ", src);
	visited[src] = true;

	for (int w = 0; w < GraphNumVertices(g); w++) {
		if (GraphIsAdjacent(g,src,w) && !visited[w]) {
			depthFirstSearchRec(g, w, visited);
		}
	}
}

void depthFirstSearch(Graph g, int src) {
	bool *visited = calloc(GraphNumVertices(g), sizeof(bool));
	depthFirstSearchRec(g, src, visited);
	free(visited);
}

