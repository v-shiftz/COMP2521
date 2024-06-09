#ifndef PQ_H
#define PQ_H

#include "Graph.h"

typedef struct pq *Pq;

Pq PqNew(void);
void PqFree(Pq pq);
void PqInsert(Pq pq, struct edge e);
struct edge PqExtract(Pq pq);
bool PqIsEmpty(Pq pq);
void PqShow(Pq pq);

#endif
