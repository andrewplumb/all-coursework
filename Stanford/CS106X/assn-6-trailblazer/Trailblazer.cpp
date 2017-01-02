/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "hashmap.h"
#include "grid.h"
#include "foreach.h"
#include "random.h"
using namespace std;

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc>
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc one, Loc two, Grid<double>& world),
             double heuristic(Loc start, Loc end, Grid<double>& world)) {

    Vector<Loc> path; /* Data structure to keep track of the shortest path */
    HashMap<Loc, Color> colors; /* Map to keep track of each location's color */
    HashMap<Loc, double> priorities; /* map to keep track of each location's distance from
                                        the start node. */
    HashMap<Loc, Loc> parents; /* map to keep track of location's parent. */
    colorCell(world, start, YELLOW);
    colors.put(start, YELLOW);
    priorities.put(start, 0);
    TrailblazerPQueue<Loc> queue;
    queue.enqueue(start, heuristic(start, end, world)); /* enqueue the start node with priority 0 +
                                                            the heuristic guess. */
    Loc curr;
    Loc next;
    while (!queue.isEmpty()){
        curr = queue.dequeueMin();
        colorCell(world, curr, GREEN); /* color the locations green as they are removed from  */
        colors.put(curr, GREEN);       /* the queue. */
        if (curr == end) break;
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                next.row = curr.row+i; /* set Loc next to be each neighbor of the dequeued location */
                next.col = curr.col+j;
                if (next.row < world.numRows() && next.row >= 0 &&    /* check to make sure Loc next */
                        next.col < world.numRows() && next.col >= 0){ /* exists within the world. */
                    if (!colors.containsKey(next)){    /* if next has not been added to the queue  */
                        colorCell(world, next, YELLOW);/* yet (so has not had it's color set) */
                        colors.put(next, YELLOW);      /* color it yellow and add it to the queue. */
                        priorities.put(next, priorities[curr] + costFn(curr, next, world));
                        parents.put(next, curr);       /* set next's parent to curr */
                        queue.enqueue(next, priorities[curr] + costFn(curr, next, world) +
                                      heuristic(next, end, world));
                    }else if (colors[next] == YELLOW){ /* if next is colored yellow, it's currently in
                                                          the queue. */
                        if (priorities[next] > priorities[curr] + costFn(curr, next, world)){
                                    /* if it's priority in the queue is higher than the distance taken
                                       to get to next through curr, lower it's priority to that distance */
                            priorities[next] = priorities[curr] + costFn(curr, next, world);
                            parents[next] = curr;
                            queue.decreaseKey(next, priorities[curr] + costFn(curr, next, world) +
                                              heuristic(next, end, world));
                        }
                    }
                }
            }
        }
    }
    path.add(curr); /* when curr == end, add curr to the path */
    while (parents.containsKey(curr)){ /* insert the current nodes parent into path at position  */
        curr = parents[curr];          /* at position 0, and set curr to it's parent, resulting */
        path.insert(0, curr);          /* in a vector of locations starting at start and ending at */
    }                                  /* end. */
    return path;
}

/*
 * function: generateNodes(int numRows, int numCols)
 * usage: Grid<Loc> locs = generateNodes(numRows, numCols)
 * -------------------
 * Generates numRows x numCols nodes, each represented by a Loc.  They are stored
 * in a Grid<Loc> representing their locations.
 */
static Grid<Loc> generateNodes(int numRows, int numCols){
    Grid<Loc> locs;
    locs.resize(numRows, numCols);
    for (int i = 0; i < numRows; i++){
        for (int j = 0; j < numCols; j++){
            Loc newLoc;
            newLoc.row = i;
            newLoc.col = j;
            locs[i][j] = newLoc;
        }
    }
    return locs;
}

/*
 * function: generateEdges(Grid<Loc>& locs)
 * usage: TrailblazerPQueue queue = generateEdges(locs)
 * -------------------
 * Creates the edges between each node of the maze.  Edges are created between edges to
 * the left and right, and top and bottom of each node.  No edges are made between diagonal
 * nodes. The edges are stored in a priority queue as they are created, and they are added to the queue
 * with a random weight between 0 and 1.
 */
static TrailblazerPQueue<Edge> generateEdges(Grid<Loc>& locs){
    TrailblazerPQueue<Edge> queue;
    for (int i = 0; i < locs.numRows()-1; i++){
        for (int j = 0; j < locs.numCols()-1; j++){
            queue.enqueue(makeEdge(locs[i][j], locs[i][j+1]), randomReal(0, 1));/* generates the edges to the
                                                                       right of each node except the last
                                                                       column and row */
            queue.enqueue(makeEdge(locs[i][j], locs[i+1][j]), randomReal(0, 1));/* generates the edges down
                                                                       from each node except the last
                                                                       column and row */
        }
    }
    for (int i = 0; i < locs.numRows()-1; i++){ /* edges down for the last column */
        queue.enqueue(makeEdge(locs[i][locs.numCols()-1], locs[i+1][locs.numCols()-1]), randomReal(0, 1));
    }
    for (int i = 0; i < locs.numCols()-1; i++){ /* edges to the right for the last row */
        queue.enqueue(makeEdge(locs[locs.numRows()-1][i], locs[locs.numRows()-1][i+1]), randomReal(0, 1));
    }
    return queue;
}


/*
 * function: initializeClusters(Grid<Loc>& locs)
 * usage: Vector<Set<Loc> > clusters = initializeClusters(locs)
 * -------------------
 * Adds creates a Set<Loc> cluster for each node, and places the node into it.  Each set is added to
 * Vector<Set<Loc> > clusters, which is a collection of all sets containing nodes.
 */
static Vector<Set<Loc> > initializeClusters(Grid<Loc>& locs){
    Vector<Set<Loc> > clusters;
    for (int i = 0; i < locs.numRows(); i++){
        for (int j = 0; j < locs.numCols(); j++){
            Set<Loc> cluster;
            cluster.add(locs[i][j]);
            clusters.add(cluster);
        }
    }
    return clusters;
}


/*
 * function: generateMST(TrailblazerPQueue<Edge> queue, Vector<Set<Loc> > clusters)
 * usage: Set<Edge> edges = generateMST(queue, clusters)
 * -------------------
 * Uses Kruskal's algorithm to generate a minimum spanning tree from the priority queue of edges.
 */
static Set<Edge> generateMST(TrailblazerPQueue<Edge> queue, Vector<Set<Loc> > clusters){
    Set<Edge> edges;
    Edge curr;
    while (clusters.size() > 1){ /* as long as there are two or more clusters remaining */
        curr = queue.dequeueMin();
        for (int j = 0; j < clusters.size(); j++){ /* search through each cluster */
            if (clusters[j].contains(curr.start) && clusters[j].contains(curr.end)) {/* If a cluster contains
                                                                                     both the start and end of
                                                                                     the edge, the edge is not
                                                                                     needed in the MST.*/
                break;
            }
            if (clusters[j].contains(curr.start) && !clusters[j].contains(curr.end)){
                                                    /* If a cluster contains one end of the edge but not the
                                                    other, find the cluster that contins the other end, merge
                                                    the two clusters together.*/
                edges.add(curr);
                for (int i = j+1; i < clusters.size(); i++){
                    if (clusters[i].contains(curr.end)){
                        clusters[j] += clusters[i];
                        clusters.remove(i);
                        break;
                    }
                }
                break;
            }
            else if (clusters[j].contains(curr.end) && !clusters[j].contains(curr.start)){
                                                    /* for each cluster, search for both the start and end of
                                                    the edge.  This ensures the minimum number of loops
                                                    through the Vector of clusters*/
                edges.add(curr);
                for (int i = j+1; i < clusters.size(); i++){
                    if (clusters[i].contains(curr.start)){
                        clusters[j] += clusters[i];
                        clusters.remove(i);
                        break;
                    }
                }
                break;
            }
        }
    }
    return edges;
}


/*
 * function: createMaze(int numRows, int numCols)
 * usage: Set<Edge> Maze = createMaze(numRows, numCols);
 * -------------------
 * Creates a random maze of numRows x numCols cells, created with a minimum spanning
 * tree.  The weights of each edge between cells is randomized between 0 and 1, and a minimum
 * spanning tree is created using Kruskal's algorithm.
 */
Set<Edge> createMaze(int numRows, int numCols) {
    Grid<Loc> locs = generateNodes(numRows, numCols);
    TrailblazerPQueue<Edge> queue = generateEdges(locs);
    Vector<Set<Loc> > clusters = initializeClusters(locs);
    Set<Edge> edges = generateMST(queue, clusters);
    return edges;
}
