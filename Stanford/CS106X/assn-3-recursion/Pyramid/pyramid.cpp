/**
 * File: draw-boxy-fractal.cpp
 * ---------------------------
 * drawBoxyFractal draws a collection of recursively layered squares
 * to form a fractal in two-dimensional space.
 */

using namespace std;

#include "console.h"
#include "vector.h"
#include "random.h"
#include "simpio.h"


static Vector<Vector<int> > createPyramid(int col);
static void printPyramid(Vector<Vector<int> >& weights);
static int weightOnKnees(int row, int col, Vector<Vector<int>>& weights);
static void printWeightSupported(Vector<Vector<int> >& weights);


int main() {
    while (true) {
        int ncols = getInteger("How many people are on the bottom row? ");

        cout << endl;

        Vector<Vector<int> > weights = createPyramid(ncols);
        printPyramid(weights);
        printWeightSupported(weights);
        string response = getLine("Make another pyramid? [y/n] ");
        if (response != "y")
            break;
        cout << endl;
    }

    return 0;
}

/*
 * function: createPyramid(int col)
 * usage: Vector<Vector<int> > weights = createPyramid(col);
 * -------------------
 * Creates a pyramid with col people on the bottom, and one fewer each row up.
 * The weight of each person is randomized between 50 and 150, and stored in the
 * Vector<Vector<int> > weigths.  weights[i][j] will give the weight of the person
 * in the ith row and jth column. weights[0][0] is the weight of the person on the
 * top of the pyramid, and weights[col-1][col-1] gives the person on the bottom
 * right.
 */
static Vector<Vector<int> > createPyramid(int col){
    Vector<Vector<int> > weights;
    for (int i = 0; i < col; i++){
        Vector<int> weightColumn;
        for (int j = 0; j < i+1; j++){
            weightColumn.add(randomInteger(50,150));
        }
        weights.add(weightColumn);
    }
    return weights;
}

/*
 * function: printPyramid(Vector<Vector<int> >& weights)
 * usage: printPyramid(Vector<Vector<int> >& weights)
 * -------------------
 * Prints the weights of each person in the pyramid, in the location
 * that person is in the pyramid.
 */
static void printPyramid(Vector<Vector<int> >& weights){
    cout << "The weights of each person in the pyramid: " << endl;
    for (int i = 0; i < weights.size(); i++){
        for (int j = 0; j < weights[i].size(); j++){
            cout << weights[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/*
 * function: weightOnKnees(int row, int col, Vector<Vector<int> >& weights)
 * usage: int weightSupported = weightOnKnees(row, col, weights)
 * -------------------
 * Calculates the total weight supported at weights[row][column] and returns
 * it as an int.
 */
static int weightOnKnees(int row, int col, Vector<Vector<int>>& weights) {
    if (row < 0 || col < 0 || col > weights[row].size()-1){
        return 0;
    }
    else{
        int weightSupported = weights[row][col]+weightOnKnees(row-1, col, weights)/2+weightOnKnees(row-1, col-1, weights)/2;
        return weightSupported;
    }
}

/*
 * function: printWeightSupported(Vector<Vector<int> >& weights)
 * usage: printWeightSupported(weights)
 * -------------------
 * Uses weightOnKnees to calculate the weight supported at each position,
 * and prints that weight at that position.
 */
static void printWeightSupported(Vector<Vector<int> >& weights){
    cout << "The weights each person in the pyramid is supporting: " << endl;
    for (int i = 0; i < weights.size(); i++){
        for (int j = 0; j < weights[i].size(); j++){
            int weightSupported = weightOnKnees(i, j, weights);
            cout << weightSupported << " ";
        }
        cout << endl;
    }
    cout << endl;
}
