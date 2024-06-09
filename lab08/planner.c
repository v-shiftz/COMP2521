// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "place.h"
#include "Pq.h"

////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */

// Helper function for planGrid1
double calculateCost(struct place a, struct place b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int planGrid1(struct place cities[], int numCities,
              struct place powerPlant,
              struct powerLine powerLines[]) {

    Graph g = GraphNew(numCities + 1);
    // Add edges between all cities and between each city and the power plant
    for (int i = 0; i < numCities; i++) {
		// Add edge between city and power plant
		double cost = calculateCost(cities[i], powerPlant);
        struct edge distanceToPlant = {i, numCities, cost};
        GraphInsertEdge(g, distanceToPlant);
        for (int j = i + 1; j < numCities; j++) {
            double distance = calculateCost(cities[i], cities[j]);
            struct edge cityTocity = {i, j, distance};
            GraphInsertEdge(g, cityTocity);
        }
        
    }

    Graph mst = GraphMst(g);

    // Extract edges from the MST to form the power lines
    int edgeCount = 0;
    for (int i = 0; i < numCities + 1; i++) {
        for (int j = i + 1; j < numCities + 1; j++) {
            double weight = GraphIsAdjacent(mst, i, j);
            if (weight > 0) {
                struct place p1;
                struct place p2;
                if (i == numCities){
                    p1 = powerPlant; 
                }else{
                    p1 = cities[i];
                }
                if (j == numCities){ 
                    p2 = powerPlant;
                }else{
                    p2 = cities[j];
                }
                struct powerLine line = {p1, p2};
                powerLines[edgeCount++] = line;
            }
        }
    }
    GraphFree(g);
    GraphFree(mst);
    return edgeCount;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(struct place cities[], int numCities,
              struct place powerPlants[], int numPowerPlants,
              struct powerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
