/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"
#include "maze-types.h"
#include "vector.h"
#include "set.h"
#include "random.h"


static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

/*
 * function: generateCells(dimension)
 * usage: Vector<cell> cells = generateCells(dimension);
 * -------------------
 * generateCells creates a cell for each row and column in the created maze
 * and stores them, in order, in a Vector of cells.  The cell in the top left
 * of the maze is stored in Vector[0], and they continue left to right, top to bottom
 * through the maze.
 */
static Vector<cell> generateCells(int dimension){
    Vector<cell> cells;
    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            cell newCell;
            newCell.row = i;
            newCell.col = j;
            cells.add(newCell);
        }
    }
    return cells;
}


/*
 * function: initializeChambers(cells)
 * usage:Vector<Set<cell> > chambers = initializeChambers(cells)
 * -------------------
 * This function creates the initial chambers.  A chamber is a Set of cells
 * that are not separated by walls. To start, each cell is it's own chamber, as
 * there are walls between all of them. The collection of chambers is stored in a
 * Vector.
 */
static Vector<Set<cell> > initializeChambers(Vector<cell>& cells){
    Vector<Set<cell> > chambers;
    for (int i = 0; i < cells.size(); i++){
        Set<cell> chamber;
        chamber.add(cells[i]);
        chambers.add(chamber);
    }
    return chambers;
}

/*
 * function: generateWalls(cells, dimension, maze)
 * usage: Vector<wall> walls = generateWalls(cells, dimension, maze);
 * -------------------
 * generateWalls creates walls between each cell of the maze. For the first
 * dimension-1 cells in a row, walls are created between that cell and the cell
 * to the right, and that cell and the cell below it. For the last cell in a row,
 * a wall is only created between it and the cell below it (there is no cell to the
 * right).  For the last row, only walls between cells and one cell to the right are
 * created, as there are no cells below. The last cell in the last row is not accessed,
 * as the wall above and to the left were already created. The walls are stored in a
 * Vector called walls. The walls are then drawn to the maze.
 */
static Vector<wall> generateWalls(Vector<cell>& cells, int dimension, MazeGeneratorView& maze){
    Vector<wall> walls;
    for (int j = 0; j < dimension-1; j++){
        for (int i = 0; i < dimension-1; i++){          // Create walls to the right and
            wall wallRight;                             // below all cells in other than those
            wallRight.one = cells[j*dimension+i];       // in the last row or column.
            wallRight.two = cells[j*dimension+i+1];
            walls.add(wallRight);
            wall wallDown;
            wallDown.one = cells[j*dimension+i];
            wallDown.two = cells[(j+1)*dimension+i];
            walls.add(wallDown);
        }
        wall lastColumnWallDown;                                        // Create walls below each cell
        lastColumnWallDown.one = cells[j*dimension+dimension-1];        // in the last column, other
        lastColumnWallDown.two = cells[(j+1)*dimension+dimension-1];    // than the last row.
        walls.add(lastColumnWallDown);
        wall lastRowWallRight;                                          // Create walls to the right of
        lastRowWallRight.one = cells[dimension*(dimension-1)+j];        // each cell in the last row,
        lastRowWallRight.two = cells[dimension*(dimension-1)+j+1];      // other than the last column.
        walls.add(lastRowWallRight);
    }
    for (int i=0; i < walls.size(); i++){
        maze.drawWall(walls[i]);
    }
    return walls;
}

/*
 * function: shuffleWalls(walls)
 * usage:  shuffleWalls(walls)
 * -------------------
 * Shuffle walls randomizes the order of the walls in Vector walls by looping
 * through the Vector and replacing the ith wall with a random wall between the
 * ith and last walls in the vector, inclusive.
 */
static void shuffleWalls(Vector<wall>& walls){
    for (int i = 0; i < walls.size()-1; i++){
        swap(walls[i], walls[randomInteger(i, walls.size()-1)]);
    }
}

/*
 * function: removeWalls(walls, chambers, maze)
 * usage: removeWalls(walls, chambers, maze)
 * -------------------
 * removeWalls removes walls from the maze.  It loops through the Vector walls
 * (which has now been shuffled, and so is in random order), and checks to see if
 * the cells each wall separate are in separate chambers.  If they are, the wall is
 * removed, and the chambers containing each cell are found.  The chambers are merged,
 * and the second chamber is deleted.
 */
static void removeWalls(Vector<wall>& walls, Vector<Set<cell> >& chambers, MazeGeneratorView& maze){
    for (wall eachWall : walls){
        for (int j = 0; j < chambers.size(); j++){
            if (chambers[j].contains(eachWall.one) && !chambers[j].contains(eachWall.two)){
                maze.removeWall(eachWall);
                for (int i = 0; i < chambers.size(); i++){
                    if (chambers[i].contains(eachWall.two)){
                        chambers[j] += chambers[i];
                        chambers.remove(i);
                        break;
                    }
                }
            }
        }
    }
}

/*
 * function: generateMaze(dimension, maze)
 * usage: generateMaze(dimension, maze)
 * -------------------
 * generageMaze sets the dimensions of maze to dimension x dimension, and
 * draws a border around it.  The cells are generated, chambers are made from
 * the cells, and the walls are created and drawn.  The walls are shuffled,
 * and then some are removed.
 */
static void generateMaze(int dimension, MazeGeneratorView& maze){
    maze.setDimension(dimension);
    maze.drawBorder();
    Vector<cell> cells = generateCells(dimension);
    Vector<Set<cell> > chambers = initializeChambers(cells);
    Vector<wall> walls = generateWalls(cells, dimension, maze);
    shuffleWalls(walls);
    removeWalls(walls, chambers, maze);
}


int main()  {
    MazeGeneratorView maze;
    while (true){
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        generateMaze(dimension, maze);
    }
    return 0;
}
