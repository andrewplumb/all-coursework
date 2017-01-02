/**
 * File: dominosa.cpp
 * ------------------
 * This animates the brute-force discovery of a solution
 * to a 2 x n dominosa board.
 */

#include <iostream>
#include <cmath>
using namespace std;

#include "simpio.h"
#include "grid.h"
#include "random.h"
#include "dominosa-graphics.h"
#include "set.h"

static bool boardSolver(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board);
static bool pairCreator(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board, int index);

static void welcome() {
	cout << "Here we'll illustrate the use of recursive backtracking to" << endl;
	cout << "discover a solution to various 2 x n Dominosa boards.  In some" << endl;
	cout << "cases there won't be any solutions, and in the cases where there are" << endl;
	cout << "multiple solutions, we'll just find one of them." << endl;
	cout << endl;
}

static int getIntegerInRange(string prompt, int low, int high) {
	while (true) {
		int response = getInteger(prompt);
		if (response == 0 || (response >= low && response <= high)) return response;
		cout << "Sorry, but I can't use that number." << endl;
	}
}

static void populateBoard(Grid<int>& board, int low, int high) {
	for (int row = 0; row < board.numRows(); row++) {
		for (int col = 0; col < board.numCols(); col++) {
			board[row][col] = randomInteger(low, high);
		}
	}
}

/*
 * function: bool canSolveBoard(DominosaDisplay& display, Grid<int>& board)
 * usage: if (canSolveBoard(display, board)) ...
 * -------------------
 * This is the wrapper function for the recursively called fuction boardSolver.  It creates
 * Vector<Set<int> > pairs, which will contain all the pairs of numbers that have been used.
 * when a new pair of numbers is created, it is checked against pairs to ensure it has
 * not already been used. It also creates the Vector<coord> numbers, which contains all the
 * locations in board, counting top to bottom, left to right.  Coords will be removed from
 * numbers as they are used in pairs.
 */
static bool canSolveBoard(DominosaDisplay& display, Grid<int>& board) {
    // replace this with your own solution, which will almost certainly be a wrapper function
    Vector<Set<int> >  pairs;
    Vector<coord> numbers;
    for (int i = 0; i < board.numCols(); i++){
        coord newCoord;
        newCoord.row = 0;
        newCoord.col = i;
        numbers.add(newCoord);
        newCoord.row = 1;
        numbers.add(newCoord);
    }

    return boardSolver(pairs, numbers, display, board);
}


/*
 * function: bool vectorContainsSet(Set<int> searchSet, Vector<Set<int> > v)
 * usage: if (vectorContainsSet(findSet, v)) ...
 * -------------------
 * Searches Vector<Set<int> > v for the Set<int> findSet.  This is used to search
 * through the Vector of used pairs to find if the newly created pair is part
 * of the vector.  Set<Set<int> > pairs would me more efficient to search through,
 * but Set<Set> is not defined.
 */
static bool vectorContainsSet(Set<int> findSet, Vector<Set<int> > v){
    for (int i = 0; i < v.size(); i++){
        if (v[i] == findSet) return true;
    }
    return false;
}

/*
 * function: bool boardSolver(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board)
 * usage: if (boardSolver(pairs, numbers, display, board)) ...
 * -------------------
 * Checks to see if the board can be solved recursively.  Returns true if the board is solveable, false if it is not.
 * boardSolver calls pairCreator, which in turn calls boardSolver on the board with the created pair removed.
 * The base case is when the size of pairs is equal to the number of columns in board, as this means all numbers
 * are in pairs.
 */
static bool boardSolver(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board){
    if (pairs.size() == board.numCols()) return true;
    if ((numbers[0].row+1 == numbers[1].row && numbers[0].col == numbers[1].col) || (numbers[0].row == numbers[1].row && numbers[0].col+1 == numbers[1].col)){
        // checks if numbers[1] is directly belor OR directly to the right of numbers[0]
        if (pairCreator(pairs, numbers, display, board, 1)){
            display.certifyPairing(numbers[0], numbers[1]); // if pairCreator returns true, this is a certified pairing.
            return true;
        }
        display.vetoProvisionalPairing(numbers[0], numbers[1]); // if pairCreator returns false,
        display.eraseProvisionalPairing(numbers[0], numbers[1]);// then this pair is vetoed.
    }
    if (2 < numbers.size() && numbers[0].row == numbers[2].row && numbers[0].col+1 == numbers[2].col){
        // after checking if numbers[2] exists, checks to see if it is directly to the right of numbers[0]
        // (it cannot be directly below numbers[0]
        if (pairCreator(pairs, numbers, display, board, 2)){
            display.certifyPairing(numbers[0], numbers[2]);
            return true;
        }
        display.vetoProvisionalPairing(numbers[0], numbers[2]);
        display.eraseProvisionalPairing(numbers[0], numbers[2]);
    }
    return false;
}

/*
 * function: bool pairCreator(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board, int index)
 * usage: if (pairCreator(pairs, numbers, display, board, index)) ...
 * -------------------
 * Pairs numbers[0] and numbers[index] (index is passed in by boardSolver) and checks if the pair already exists
 * in Vector<Set<int> > pairs.  If it does not, the new pair is added to pairs and the coordinates used in it
 * are removed from numbers.  boardSovler is called again, using the new Vector<coord> numbers that has had the
 * coordinates used removed.  If the new pair does exist in pairs, returns false.
 */
static bool pairCreator(Vector<Set<int> > pairs, Vector<coord> numbers, DominosaDisplay& display, Grid<int>& board, int index){
    display.provisonallyPair(numbers[0], numbers[index]);
    Set<int> newPair;
    newPair.add(board.get(numbers[0].row, numbers[0].col));
    newPair.add(board.get(numbers[index].row, numbers[index].col));
    if (!vectorContainsSet(newPair, pairs)){
        pairs.add(newPair);
        numbers.remove(index);
        numbers.remove(0);
        if (boardSolver(pairs, numbers, display, board)) return true;
    }
    return false;
}

int main() {
	DominosaDisplay display;
	welcome();
	while (true) {
        int numColumns = getIntegerInRange("How many columns? [0 to exit]: ", 9, 25);
		if (numColumns == 0) break;
		Grid<int> board(2, numColumns);
		populateBoard(board, 1, ceil(2 * sqrt((double) numColumns)));
		display.drawBoard(board);
		if (canSolveBoard(display, board)) {
			cout << "The board can be solved, and one such solution is drawn above." << endl;
		} else {
			cout << "This board you see can't be solved." << endl;
        }
	}
	
	cout << "Okay, thanks for watching, and come back soon." << endl;
    cout << "Click the mouse anywhere in the window to exit." << endl;
	return 0;
}
