// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Pq.h"

// DO NOT modify this struct
struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, struct edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight >= 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////

// Helper function for isConnected
static void dfs(const Graph g, int v, bool visited[]) {
    visited[v] = true;
    for (int w = 0; w < g->nV; w++) {
        if (g->edges[v][w] != 0.0 && !visited[w]) {
            dfs(g, w, visited);
        }
    }
}

// Helper function to check if graphs are connected
static bool isConnected(Graph g) {
    bool *visited = calloc(g->nV, sizeof(bool));
    if (visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    
    dfs(g, 0, visited);
    
    for (int i = 0; i < g->nV; i++) {
        if (!visited[i]) {
            free(visited);
            return false;
        }
    }
    
    free(visited);
    return true;
}

Graph GraphMst(Graph g) {
    if (!isConnected(g)) {
        printf("No minimum spanning trees\n");
        return NULL;
    }
    
    int nV = GraphNumVertices(g);
    bool *inMst = malloc(nV * sizeof(bool));
    if (inMst == NULL){
        fprintf(stderr, "Memory allocation failed lmao\n");
        return NULL;
    }

    for (int i = 0; i < nV; i++){
        inMst[i] = false;
    }
    Graph mst = GraphNew(nV);
    Pq pq = PqNew();

    inMst[0] = true;
    for (int i = 0; i <nV; i++){
        if (g->edges[0][i] != 0.0){
            struct edge e = {0, i, g->edges[0][i]};
            PqInsert(pq, e);
        }
    }

    while (!PqIsEmpty(pq)) {
        struct edge e = PqExtract(pq);
        if (!inMst[e.w]) {
            GraphInsertEdge(mst, e);
            inMst[e.w] = true;
            for (int i = 0; i < nV; i++) {
                if (!inMst[i] && g->edges[e.w][i] != 0.0) {
                    struct edge newEdge = {e.w, i, g->edges[e.w][i]};
                    PqInsert(pq, newEdge);
                }
            }
        }
    }
    PqFree(pq);
    free(inMst);

    return mst;
}

////////////////////////////////////////////////////////////////////////

static bool validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

